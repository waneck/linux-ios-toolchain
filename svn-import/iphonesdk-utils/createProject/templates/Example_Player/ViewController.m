//
//  ViewController.m
//  MPPlayer
//

#import "ViewController.h"

@implementation ViewController

- (void)loadMoviePlayer
{  
    // Play movie from the bundle
    NSString *path = [[NSBundle mainBundle] pathForResource:@"Movie-1" ofType:@"mp4" inDirectory:nil];
   
    // Create custom movie player   
    moviePlayer = [[[CustomMPViewController alloc] initWithPath:path] autorelease];

    // Show the movie player as modal
    [self presentModalViewController:moviePlayer animated:YES];

    // Prep and play the movie
    [moviePlayer readyPlayer];    
}

- (void)buttonPressed:(UIButton *)button
{
    // If pressed, play movie
    if (button == playButton)
        [self loadMoviePlayer];	
}

- (void)loadView
{
    // Setup the view
    [self setView:[[[UIView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]] autorelease]];
    [[self view] setBackgroundColor:[UIColor grayColor]];
    [[self view] setUserInteractionEnabled:YES];

	// Add play button 
    playButton = [[UIButton alloc] initWithFrame:CGRectMake(53, 212, 214, 36)];    
    [playButton setBackgroundImage:[UIImage imageNamed:@"playButton.png"] forState:UIControlStateNormal];
    [playButton addTarget:self action:@selector(buttonPressed:) forControlEvents: UIControlEventTouchUpInside];
    [[self view] addSubview:playButton];  
}

- (void)dealloc 
{
    [playButton release];  
    [super dealloc];
}

@end
