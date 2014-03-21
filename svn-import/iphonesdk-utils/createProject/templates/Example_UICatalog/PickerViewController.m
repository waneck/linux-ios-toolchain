/*

File: PickerViewController.m
Abstract: The view controller for hosting the UIPickerView of this sample.

Version: 1.7

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

*/

#import "PickerViewController.h"
#import "CustomPicker.h"
#import "Constants.h"

@implementation PickerViewController

- (id)init
{
	self = [super init];
	if (self)
	{
		// this title will appear in the navigation bar
		self.title = NSLocalizedString(@"PickerTitle", @"");
	}
	
	return self;
}

// return the picker frame based on its size, positioned at the bottom of the page
- (CGRect)pickerFrameWithSize:(CGSize)size
{
	CGRect screenRect = [[UIScreen mainScreen] applicationFrame];
	CGRect pickerRect = CGRectMake(	0.0,
									screenRect.size.height - kToolbarHeight - 44.0 - size.height,
									size.width,
									size.height);
	return pickerRect;
}

#pragma mark 
#pragma mark UIPickerView
#pragma mark
- (void)createPicker
{
	pickerViewArray = [[NSArray arrayWithObjects:
							@"John Appleseed", @"Chris Armstrong", @"Serena Auroux",
							@"Susan Bean", @"Luis Becerra", @"Kate Bell", @"Alain Briere",
							nil] retain];
	// note we are using CGRectZero for the dimensions of our picker view,
	// this is because picker views have a built in optimum size,
	// you just need to set the correct origin in your view.
	//
	// position the picker at the bottom
	myPickerView = [[UIPickerView alloc] initWithFrame:CGRectZero];
	CGSize pickerSize = [myPickerView sizeThatFits:CGSizeZero];
	myPickerView.frame = [self pickerFrameWithSize:pickerSize];

	myPickerView.autoresizingMask = UIViewAutoresizingFlexibleWidth;
	myPickerView.delegate = self;
	myPickerView.showsSelectionIndicator = YES;	// note this is default to NO
	
	// add this picker to our view controller, initially hidden
	myPickerView.hidden = YES;
	[self.view addSubview:myPickerView];
}

#pragma mark 
#pragma mark UIPickerView - Date/Time
#pragma mark
- (void)createDatePicker
{
	datePickerView = [[UIDatePicker alloc] initWithFrame:CGRectZero];
	datePickerView.autoresizingMask = UIViewAutoresizingFlexibleWidth;
	datePickerView.datePickerMode = UIDatePickerModeTime;
	
	// note we are using CGRectZero for the dimensions of our picker view,
	// this is because picker views have a built in optimum size,
	// you just need to set the correct origin in your view.
	//
	// position the picker at the bottom
	CGSize pickerSize = [myPickerView sizeThatFits:CGSizeZero];
	datePickerView.frame = [self pickerFrameWithSize:pickerSize];
	
	// add this picker to our view controller, initially hidden
	datePickerView.hidden = YES;
	[self.view addSubview:datePickerView];
}

#pragma mark 
#pragma mark UIPickerView - Custom Picker
#pragma mark
- (void)createCustomPicker
{
	customPickerView = [[CustomPicker alloc] initWithFrame:CGRectZero];
	customPickerView.autoresizingMask = UIViewAutoresizingFlexibleWidth;
	
	// note we are using CGRectZero for the dimensions of our picker view,
	// this is because picker views have a built in optimum size,
	// you just need to set the correct origin in your view.
	//
	// position the picker at the bottom
	CGSize pickerSize = [myPickerView sizeThatFits:CGSizeZero];
	customPickerView.frame = [self pickerFrameWithSize:pickerSize];
	
	customPickerView.showsSelectionIndicator = YES;
	
	// add this picker to our view controller, initially hidden
	customPickerView.hidden = YES;
	[self.view addSubview:customPickerView];
}

- (void)showPicker:(UIView *)picker
{
	// hide the current picker and show the new one
	if (currentPicker)
	{
		currentPicker.hidden = YES;
		label.text = @"";
	}
	picker.hidden = NO;
	
	currentPicker = picker;	// remember the current picker so we can remove it later when another one is chosen
}

- (void)createButtonBar
{
	// create the button bar
	buttonBar = [UIToolbar new];
	buttonBar.barStyle = UIBarStyleBlackOpaque;
	
	// position the button bar's frame
	CGFloat toolbarHeight = kToolbarHeight + 4.0;
	CGRect mainViewBounds = self.view.bounds;
	[buttonBar setFrame:CGRectMake(CGRectGetMinX(mainViewBounds),
								   CGRectGetMinY(mainViewBounds) + CGRectGetHeight(mainViewBounds) - (toolbarHeight * 2.0),
								   CGRectGetWidth(mainViewBounds),
								   toolbarHeight)];
	
	// add a segmented control to the button bar
	buttonBarSegmentedControl = [[UISegmentedControl alloc] initWithItems:
									[NSArray arrayWithObjects:@"UIPicker", @"UIDatePicker", @"Custom", nil]];
	[buttonBarSegmentedControl addTarget:self action:@selector(togglePickers:) forControlEvents:UIControlEventValueChanged];
	buttonBarSegmentedControl.selectedSegmentIndex = 0.0;	// start by showing the normal picker
	buttonBarSegmentedControl.segmentedControlStyle = UISegmentedControlStyleBar;
    buttonBarSegmentedControl.tintColor = [UIColor darkGrayColor];
	buttonBarSegmentedControl.backgroundColor = [UIColor clearColor];

	UIBarButtonItem *segItem = [[UIBarButtonItem alloc] initWithCustomView:buttonBarSegmentedControl];	
	buttonBar.items = [NSArray arrayWithObject:segItem];
	[segItem release];
	
	[self.view addSubview:buttonBar];
	
	CGRect buttonBarFrame = [buttonBar frame];
	CGRect segmentedControlFrame = CGRectMake(kLeftMargin,
											  kTopMargin,
											  buttonBarFrame.size.width - kLeftMargin,
											  30.0);
    buttonBarSegmentedControl.frame = segmentedControlFrame;
	
	// start by showing the normal picker
	buttonBarSegmentedControl.selectedSegmentIndex = 0.0;
}	

- (void)loadView
{		
	CGRect screenRect = [[UIScreen mainScreen] applicationFrame];
	
	// setup our parent content view and embed it to your view controller
	UIView *contentView = [[UIView alloc] initWithFrame:screenRect];
	contentView.backgroundColor = [UIColor blackColor];
	contentView.autoresizesSubviews = YES;
	contentView.autoresizingMask = UIViewAutoresizingFlexibleWidth;
	
	self.view = contentView;
	[contentView release];
	
	[self createPicker];	
	[self createDatePicker];
	[self createCustomPicker];
	
	[self createButtonBar];
	
	// label for picker selection output, place it right above the picker
	CGRect labelFrame = CGRectMake(	kLeftMargin,
									myPickerView.frame.origin.y - kTextFieldHeight,
									self.view.bounds.size.width - (kRightMargin * 2.0),
									kTextFieldHeight);
	label = [[UILabel alloc] initWithFrame:labelFrame];
    label.font = [UIFont systemFontOfSize: 14];
    IF_PRE_IOS6 (
      label.textAlignment = UITextAlignmentCenter;
    )
    IF_IOS6_OR_GREATER (
      label.textAlignment = NSTextAlignmentCenter;
    )
	label.textColor = [UIColor whiteColor];
	label.backgroundColor = [UIColor clearColor];
	[self.view addSubview:label];
	
	// create the segmented control to control the UIDatePicker's style (date/time/datetime)
	pickerStyleSegmentedControl = [[UISegmentedControl alloc] initWithItems:
										[NSArray arrayWithObjects:@"1", @"2", @"3", @"4", nil]];
	[pickerStyleSegmentedControl addTarget:self action:@selector(togglePickerStyle:) forControlEvents:UIControlEventValueChanged];
	pickerStyleSegmentedControl.segmentedControlStyle = UISegmentedControlStyleBar;
	pickerStyleSegmentedControl.tintColor = [UIColor darkGrayColor];
    pickerStyleSegmentedControl.backgroundColor = [UIColor clearColor];
	[pickerStyleSegmentedControl sizeToFit];
	pickerStyleSegmentedControl.hidden = YES;
	
	CGRect segmentedControlFrame = CGRectMake(kRightMargin,
											  kTopMargin + kTextFieldHeight + 5,
											  screenRect.size.width - (kRightMargin * 2.0),
											  kSegmentedControlHeight);
    pickerStyleSegmentedControl.frame = segmentedControlFrame;
	[self.view addSubview:pickerStyleSegmentedControl];
	
	// create the selection label for our UIDatePicker segmented control
	labelFrame = CGRectMake(0,
							kTopMargin,
							self.view.bounds.size.width,
							kTextFieldHeight);
	segmentLabel = [[UILabel alloc] initWithFrame:labelFrame];
    segmentLabel.font = [UIFont systemFontOfSize: 14];
    IF_PRE_IOS6 (
      segmentLabel.textAlignment = UITextAlignmentCenter;
    )
    IF_IOS6_OR_GREATER (
      segmentLabel.textAlignment = NSTextAlignmentCenter;
    )
	segmentLabel.textColor = [UIColor whiteColor];
	segmentLabel.backgroundColor = [UIColor clearColor];
	segmentLabel.hidden = YES;
	[self.view addSubview:segmentLabel];
	
	// start by showing the normal picker
	buttonBarSegmentedControl.selectedSegmentIndex = 0.0;
}

- (void)dealloc
{
	[pickerViewArray release];
	[myPickerView release];
	[datePickerView release];
	[label release];
	[customPickerView release];
	
	[pickerStyleSegmentedControl release];
	[segmentLabel release];
	
	[buttonBarSegmentedControl release];
	[buttonBar release];
	
	[super dealloc];
}

- (void)togglePickerStyle:(id)sender
{
	UISegmentedControl *segControl = sender;
	switch (segControl.selectedSegmentIndex)
	{
		case 0:	// Time
		{
			datePickerView.datePickerMode = UIDatePickerModeTime;
			segmentLabel.text = @"UIDatePickerModeTime";
			break;
		}
		case 1: // Date
		{	
			datePickerView.datePickerMode = UIDatePickerModeDate;
			segmentLabel.text = @"UIDatePickerModeDate";
			break;
		}
		case 2:	// Date & Time
		{
			datePickerView.datePickerMode = UIDatePickerModeDateAndTime;
			segmentLabel.text = @"UIDatePickerModeDateAndTime";
			break;
		}
		case 3:	// Counter
		{
			datePickerView.datePickerMode = UIDatePickerModeCountDownTimer;
			segmentLabel.text = @"UIDatePickerModeCountDownTimer";
			break;
		}
	}
	
	// in case we previously chose the Counter style picker, make sure
	// the current date is restored
	NSDate *today = [NSDate date];
	datePickerView.date = today;
}

- (void)togglePickers:(id)sender
{
	UISegmentedControl *segControl = sender;
	switch (segControl.selectedSegmentIndex)
	{
		case 0:	// UIPickerView
		{
			pickerStyleSegmentedControl.hidden = YES;
			segmentLabel.hidden = YES;
			[self showPicker:myPickerView];
			break;
		}
		case 1: // UIDatePicker
		{	
			// start by showing the time picker
			
			// initially set the picker style to "time" format
			pickerStyleSegmentedControl.selectedSegmentIndex = 0.0;
			pickerStyleSegmentedControl.hidden = NO;
			segmentLabel.hidden = NO;
			[self showPicker:datePickerView];
			break;
		}
	
		case 2:	// Custom
		{
			pickerStyleSegmentedControl.hidden = YES;
			segmentLabel.hidden = YES;
			[self showPicker:customPickerView];	
			break;
		}
	}
}

- (void)pickerAction:(id)sender
{
	if (myPickerView != currentPicker)
		[self showPicker:myPickerView];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	// if you want to only support portrait mode, do this
	//return (interfaceOrientation == UIInterfaceOrientationPortrait);
	
	return YES;
}


#pragma mark -
#pragma mark PickerView delegate methods

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
	// report the selection to the UI label
	label.text = [NSString stringWithFormat:@"%@ - %d",
		[pickerViewArray objectAtIndex:[pickerView selectedRowInComponent:0]], [pickerView selectedRowInComponent:1]];
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
	NSString *returnStr;
	if (component == 0)
	{
		returnStr = [pickerViewArray objectAtIndex:row];
	}
	else
	{
		returnStr = [[NSNumber numberWithInt:row] stringValue];
	}
	return returnStr;
}

- (CGFloat)pickerView:(UIPickerView *)pickerView widthForComponent:(NSInteger)component
{
	CGFloat componentWidth;
	if (component == 0)
		componentWidth = 240.0;	// first column size is wider to hold names
	else
		componentWidth = 40.0;	// second column is narrower to show numbers
	return componentWidth;
}

- (CGFloat)pickerView:(UIPickerView *)pickerView rowHeightForComponent:(NSInteger)component
{
	return 40.0;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
	return [pickerViewArray count];
}

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
	return 2;
}


#pragma mark UIViewController delegate methods

// called after this controller's view was dismissed, covered or otherwise hidden
- (void)viewWillDisappear:(BOOL)animated
{
	currentPicker.hidden = YES;
	
	// restore the nav bar and status bar color to default
	self.navigationController.navigationBar.barStyle = UIBarStyleDefault;
	[UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleDefault;
}

// called after this controller's view will appear
- (void)viewWillAppear:(BOOL)animated
{
	[self togglePickers:buttonBarSegmentedControl];	// make sure the last picker is still showing
	
	// for aesthetic reasons (the background is black), make the nav bar black for this particular page
	self.navigationController.navigationBar.barStyle = UIBarStyleBlackOpaque;
	
	// match the status bar with the nav bar
	[UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleBlackOpaque;
}

@end

