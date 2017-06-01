# Deprecated
If you are interested in a mac/iOs toolchain for linux, please check out [cctools-port](https://github.com/tpoechtrager/cctools-port/), which is being actively developed and supported

*This project is a fork of [ios-toolchain-based-on-clang](http://code.google.com/p/ios-toolchain-based-on-clang-for-linux/). All the credits go to the original authors! If you like this, support the original project/authors and tell them how awesome they are!*

[![Build Status](https://travis-ci.org/waneck/linux-ios-toolchain.svg?branch=master)](https://travis-ci.org/waneck/linux-ios-toolchain)

## What
Mac and iOS toolchain for linux. By providing a compiler, assembler, linker, debugger and some utilities, you will be able to cross compile applications to an iPhone and to a Mac directly from a Linux box.

## What changed
This project was forked to serve as a mirror to the original project, and also to enjoy the benefits of git, easily allowing linux users to try on their distros and contribute. Also, the original project depended on some external links / tarballs to be available. To avoid the risk of the links becoming dead, all the files needed for this project are contained within this project. The changes from the original svn project will be merged back.
The wrapper around clang was also changed so it can be a simple shell script.

## What works
I've fully tested using clang on both iOS and Mac `x86_64` targets (the [haxe](http://haxe.org) nightly builds are built using it, along with [cross-ocaml](https://github.com/waneck/cross-ocaml)) on `Ubuntu 13.10`. The basics of debugging with lldb on iOS was tested, and uploading the project to iOS (on a jailbroken device) was tested on both iOS 5.0 and 6.0 . Deploying to iOS 7.0 has still not been tested.
The rest is expected to work but still hasn't been tested by me.

## Does it really work?
Yes. `:D`

## How
An effort is being made to make the project compilation as painless as possible. If you're on Ubuntu 13.10, you should be able to compile running the following commands:

```bash
git clone --recursive https://github.com/waneck/linux-ios-toolchain.git
cd linux-ios-toolchain
sudo apt-get install gcc g++ clang libclang-dev uuid-dev libssl-dev libpng12-dev libicu-dev bison flex libsqlite3-dev libtool llvm-dev libxml2-dev automake pkg-config
./configure
make
sudo make install
```

Users from other distributions are encouraged to share/submit an issue so we can get it to work with their setup.

## How to hxcpp/haxe
SOON
