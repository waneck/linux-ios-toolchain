#import "BBWeeAppController-Protocol.h"

static NSBundle *_TEMPLATE_PROJECTWeeAppBundle = nil;

@interface TEMPLATE_PROJECTController: NSObject <BBWeeAppController> {
  UIView *_view;
  UIImageView *_backgroundView;
}
@property (nonatomic, retain) UIView *view;
@end

@implementation TEMPLATE_PROJECTController
@synthesize view = _view;

+ (void)initialize {
  _TEMPLATE_PROJECTWeeAppBundle = [NSBundle bundleForClass:[self class]];
}

- (id)init {
  if((self = [super init]) != nil) {
		
  } return self;
}

- (void)loadFullView {
  // Add subviews to _backgroundView (or _view) here.
}

- (void)loadPlaceholderView {
  // This should only be a placeholder - it should not connect to any servers or perform any intense
  // data loading operations.
  //
  // All widgets are 316 points wide. Image size calculations match those of the Stocks widget.
  _view = [[UIView alloc] initWithFrame:(CGRect){CGPointZero, {316.f, [self viewHeight]}}];
  _view.autoresizingMask = UIViewAutoresizingFlexibleWidth;

  UIImage *bgImg = [UIImage imageWithContentsOfFile:@"/System/Library/WeeAppPlugins/StocksWeeApp.bundle/WeeAppBackground.png"];
  UIImage *stretchableBgImg = [bgImg stretchableImageWithLeftCapWidth:floorf(bgImg.size.width / 2.f) topCapHeight:floorf(bgImg.size.height / 2.f)];
  _backgroundView = [[UIImageView alloc] initWithImage:stretchableBgImg];
  _backgroundView.frame = CGRectInset(_view.bounds, 2.f, 0.f);
  _backgroundView.autoresizingMask = UIViewAutoresizingFlexibleWidth;
  [_view addSubview:_backgroundView];

  int screenwidth = [[UIScreen mainScreen] bounds].size.width;
  CGRect frame = CGRectMake(20,10,screenwidth-40,30);
  UILabel *label = [[UILabel alloc] initWithFrame:frame];
  label.backgroundColor = [UIColor clearColor];
  [label setTextColor:[UIColor redColor]];
  [label setTextAlignment:UITextAlignmentCenter];
  [label setText:@"TEMPLATE_PROJECT"];

  [_view addSubview:label];
}

- (float)viewHeight {
  return 71.f;
}

@end














