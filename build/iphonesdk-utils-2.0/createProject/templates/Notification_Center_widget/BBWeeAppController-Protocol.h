#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@protocol BBWeeAppController <NSObject>
@required
- (id)view;
@optional
- (void)loadPlaceholderView;
- (void)loadFullView;
- (void)loadView;
- (void)unloadView;
- (void)clearShapshotImage;
- (id)launchURL;
- (id)launchURLForTapLocation:(CGPoint)tapLocation;
- (float)viewHeight;
- (void)viewWillAppear;
- (void)viewDidAppear;
- (void)viewWillDisappear;
- (void)viewDidDisappear;
- (void)willAnimateRotationToInterfaceOrientation:(int)interfaceOrientation;
- (void)willRotateToInterfaceOrientation:(int)interfaceOrientation;
- (void)didRotateFromInterfaceOrientation:(int)interfaceOrientation;
@end
