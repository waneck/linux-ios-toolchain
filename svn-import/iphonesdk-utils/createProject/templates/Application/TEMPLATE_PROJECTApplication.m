#import "TEMPLATE_PROJECTApplication.h"

@implementation TEMPLATE_PROJECTApplication
@synthesize window = _window;
- (void)applicationDidFinishLaunching:(UIApplication *)application {
  _window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
  _viewController = [[RootViewController alloc] init];
  [_window setRootViewController:_viewController];
  [_window makeKeyAndVisible];
}

@end

