//
//  AppDelegate.h
//  MPPlayer
//

#import <UIKit/UIKit.h>
#import "ViewController.h"

@interface AppDelegate : NSObject <UIApplicationDelegate> {

    UIWindow *window;
    ViewController *vc;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) ViewController *vc;

@end
