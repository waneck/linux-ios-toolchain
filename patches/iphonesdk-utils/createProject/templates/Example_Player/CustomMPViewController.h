//
//  CustomMPViewController.h
//  MPPlayer
//

#import <UIKit/UIKit.h>
#import <MediaPlayer/MediaPlayer.h>

@interface CustomMPViewController : UIViewController 
{
    MPMoviePlayerController *mp;
    NSURL *movieURL;
}

- (id)initWithPath:(NSString *)moviePath;
- (void)readyPlayer;

@end
