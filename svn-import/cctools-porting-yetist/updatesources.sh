#!/bin/sh
cwd=`pwd`
distdir="${cwd}/workspace"
[ -d ${distdir} ] || mkdir ${distdir}

packages=("cctools-836" "ld64-134.9" "libobjc2-1.6.1")
sources=("http://www.opensource.apple.com/tarballs/cctools/%s-%s.tar.gz"
"http://www.opensource.apple.com/tarballs/ld64/%s-%s.tar.gz"
"http://download.gna.org/gnustep/%s-%s.tar.bz2"
)
md5sums=("62a3b204265170c39333c9d81ef6249d"
"feb205b72ea38bb4da4f4429ab88fc1b"
"611ef55ed15ea9c1a7b4b72807e41210")

GENINTEG=1
checksum(){
	local realmd5=`md5sum ${tarball} |awk -F' ' '{print $1}'`
	if [ ! -f $tarball ];then
		return 0
	else
		if [ "x$realmd5" == "x$expectedsum" ];then
			return 1
		else
			return 0
		fi
	fi
}

generate_checksums(){
	#echo "Validating source files with md5sums..."
	echo -n "\"" >&2
	local realmd5=`md5sum ${tarball} |awk -F' ' '{print $1}'`
	echo "${realmd5}\""
}

download_sources(){
	echo -n "Download ${tarball} ... "
	local n=0
	while checksum -lt 1 && test $n -lt 3 ; do
		[ -f $tarball ] || rm -f $tarball
		wget -O $tarball $url
		n=$((n + 1))
	done
	if checksum -lt 1;then
		echo "FAILED"
		exit 0
	else
		echo "PASS"
	fi
}

extract_sources(){
	tmpdir=`mktemp -d`
	local package=${pkgname}-${pkgver}
	echo "uncompress $package files..."
	tar --exclude="${package}/${package}" -xf ${tarball} -C ${tmpdir} >/dev/null 2>&1
	find ${tmpdir} -type f -a \( -name "*.h" -o -name "*.hpp" -o  -name "*.c" -o -name "*.cc" -o  -name "*.cpp" \) -exec sed -i 's/#import/#include/g' {} \;
	for i in `find ${tmpdir} -name "*.h" -o -name "*.hpp" -o  -name "*.c" -o  -name "*.cc" -o  -name "*.cpp" -o -name "*.m" -o -name "*.S"`;do
		local pkgdir="${tmpdir}/${package}"
		case "$pkgname" in
			ld64|libobjc2)
				local dst="${distdir}/${pkgname}/${i#$pkgdir/}"
				;;
			*)
				local dst="${distdir}/${i#$pkgdir/}"
		esac
		echo $dst
		install -Dm644 $i $dst
	done
	rm -rf ${tmpdir}
}

main(){
	local idx=0
	echo -e "VERSION\n============" > ${distdir}/VERSION
	for netfile in "${packages[@]}"; do
		local pkgname=${netfile%-*}
		local pkgver=${netfile#*-}
		local urlformat=$(tr '[:upper:]' '[:lower:]' <<< "${sources[$idx]}")
		local expectedsum=$(tr '[:upper:]' '[:lower:]' <<< "${md5sums[$idx]}")
		local url=`printf $urlformat $pkgname $pkgver`
		local tarball=`basename $url`
		download_sources
		extract_sources
		echo "${pkgname}-${pkgver}, url: $url" >> ${distdir}/VERSION
		idx=$((idx + 1))
	done
}

main
