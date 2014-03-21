#import "RootViewController.h"

@implementation RootViewController
- (void)loadView {
  self.view = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
  self.view.backgroundColor = [UIColor redColor];

  UIButton *roundedRectButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
  CGRect buttonRect = CGRectMake(100,50,100,35);
  [roundedRectButton setFrame:buttonRect];
  [roundedRectButton setTitle:NSLocalizedString(@"Hello World", @"") forState:UIControlStateNormal];
  [roundedRectButton addTarget:self action:@selector(showAlert:) forControlEvents:UIControlEventTouchUpInside];
  [self.view addSubview:roundedRectButton]; 
}


- (void)showAlert:(id)sender
{
  UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"UIAlertView", @"") message:NSLocalizedString(@"<This is Alert>",@"") delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
  [alert show];
}
@end











