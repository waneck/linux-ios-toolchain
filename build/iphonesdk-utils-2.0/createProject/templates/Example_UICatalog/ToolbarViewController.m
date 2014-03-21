/*

File: ToolbarViewController.m
Abstract: The view controller for hosting the UIToolbar and UIBarButtonItem
features of this sample.

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

#import "ToolbarViewController.h"
#import "AppDelegate.h"
#import "Constants.h"

@implementation ToolbarViewController

- (id)init
{
	self = [super init];
	if (self)
	{
		// this title will appear in the navigation bar
		self.title = NSLocalizedString(@"ToolbarTitle", @"");
	}
	return self;
}


- (void)dealloc
{	
    [toolbar release];
	[pickerViewArray release];
	[styleSegmentedControl release];
	[super dealloc];
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

- (void)createPicker
{
	// this list appears in the UIPickerView to pick the system's UIBarButtonItem
	pickerViewArray = [[NSArray arrayWithObjects:
						@"Done",
						@"Cancel",
						@"Edit",  
						@"Save",  
						@"Add",
						@"FlexibleSpace",
						@"FixedSpace",
						@"Compose",
						@"Reply",
						@"Action",
						@"Organize",
						@"Bookmarks",
						@"Search",
						@"Refresh",
						@"Stop",
						@"Camera",
						@"Trash",
						@"Play",
						@"Pause",
						@"Rewind",
						@"FastForward",
						nil] retain];
	
	// note we are using CGRectZero for the dimensions of our picker view,
	// this is because picker views have a built in optimum size,
	// you just need to set the correct origin in your view.
	//
	// position the picker at the bottom
	UIPickerView *myPickerView = [[UIPickerView alloc] initWithFrame:CGRectZero];
	CGSize pickerSize = [myPickerView sizeThatFits:CGSizeZero];
	myPickerView.frame = [self pickerFrameWithSize:pickerSize];
	
	myPickerView.autoresizingMask = UIViewAutoresizingFlexibleWidth;
	myPickerView.delegate = self;
	myPickerView.showsSelectionIndicator = YES;	// note this is default to NO
	
	// add this picker to our view controller, initially hidden
	[self.view addSubview:myPickerView];
	[myPickerView release];
}

- (void)createToolbarItems
{	
	// match each of the toolbar item's style match the selection in the "UIBarButtonItemStyle" segmented control
	UIBarButtonItemStyle style = [styleSegmentedControl selectedSegmentIndex];

	// create the system-defined "OK or Done" button
    UIBarButtonItem *systemItem = [[UIBarButtonItem alloc]
									initWithBarButtonSystemItem:currentSystemItem
									target:self action:@selector(action:)];
	systemItem.style = style;
	
	// flex item used to separate the left groups items and right grouped items
	UIBarButtonItem *flexItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace
																			   target:nil
																			   action:nil];
	
	// create a special tab bar item with a custom image and title
	UIBarButtonItem *infoItem = [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"segment_tools.png"]
																  style:style
																 target:self
																 action:@selector(action:)];
	
	// create a bordered style button with custom title
	UIBarButtonItem *customItem = [[UIBarButtonItem alloc] initWithTitle:@"Item"
																	style:style	// note you can use "UIBarButtonItemStyleDone" to make it blue
																   target:self
																   action:@selector(action:)];
	
	NSArray *items = [NSArray arrayWithObjects: systemItem, flexItem, customItem, infoItem, nil];
	[toolbar setItems:items animated:NO];
	
	[systemItem release];
	[flexItem release];
	[infoItem release];
	[customItem release];
}

- (void)loadView
{
	CGRect screenRect = [[UIScreen mainScreen] applicationFrame];
	
	// setup our parent content view and embed it to your view controller
	//
	UIView *contentView = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
	contentView.autoresizesSubviews = YES;
	contentView.autoresizingMask = (UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight);
	contentView.backgroundColor = [UIColor groupTableViewBackgroundColor];	// use the table view background color
	self.view = contentView;
	[contentView release];
	
	// create the segmented control to control the style content of the bottom UIToolbar
	//
	styleSegmentedControl = [[UISegmentedControl alloc] initWithItems:
												[NSArray arrayWithObjects:@"Plain", @"Bordered", @"Done", nil]];
	[styleSegmentedControl addTarget:self action:@selector(toggleStyle:) forControlEvents:UIControlEventValueChanged];
	styleSegmentedControl.segmentedControlStyle = UISegmentedControlStyleBar;
    styleSegmentedControl.backgroundColor = [UIColor clearColor];
	[styleSegmentedControl sizeToFit];
	styleSegmentedControl.selectedSegmentIndex = 0;
	CGRect segmentedControlFrame = CGRectMake(kRightMargin,
											  kTweenMargin + 20.0,
											  screenRect.size.width - (kRightMargin * 2.0),
											  kSegmentedControlHeight);
    styleSegmentedControl.frame = segmentedControlFrame;
	[self.view addSubview:styleSegmentedControl];
	[styleSegmentedControl release];
	
	// create the label for our UIBarButtonItemStyle segmented control
	//
	CGRect labelFrame = CGRectMake(	0.0,
									kTweenMargin,
									self.view.bounds.size.width,
									20.0);
	UILabel *label = [[UILabel alloc] initWithFrame:labelFrame];
    label.font = [UIFont systemFontOfSize: 12];
	label.text = @"UIBarButtonItemStyle";
    IF_PRE_IOS6 (
      label.textAlignment = UITextAlignmentCenter;
    )
    IF_IOS6_OR_GREATER (
      label.textAlignment = NSTextAlignmentCenter;
    )
	label.textColor = [UIColor blackColor];
	label.backgroundColor = [UIColor clearColor];
	[self.view addSubview:label];
	[label release];
	
	// create the segmented control to control the style content of the bottom UIToolbar
	//
	UISegmentedControl *segmentedControl = [[UISegmentedControl alloc] initWithItems:
											[NSArray arrayWithObjects:@"Default", @"Black", @"Translucent", nil]];
	[segmentedControl addTarget:self action:@selector(toggleBarStyle:) forControlEvents:UIControlEventValueChanged];
	segmentedControl.segmentedControlStyle = UISegmentedControlStyleBar;
    segmentedControl.backgroundColor = [UIColor clearColor];
	[segmentedControl sizeToFit];
	segmentedControl.selectedSegmentIndex = 0;
	segmentedControlFrame = CGRectMake(	kRightMargin,
										75.0 + 20.0,
										screenRect.size.width - (kRightMargin * 2.0),
										kSegmentedControlHeight);
    segmentedControl.frame = segmentedControlFrame;
	[self.view addSubview:segmentedControl];
	[segmentedControl release];
	
	// create the label for our UIBarStyle segmented control
	//
	labelFrame = CGRectMake(0.0, 70.0, self.view.bounds.size.width, kTextFieldHeight);
	label = [[UILabel alloc] initWithFrame:labelFrame];
    label.font = [UIFont systemFontOfSize: 12];
	label.text = @"UIBarStyle";
    IF_PRE_IOS6 (
      label.textAlignment = UITextAlignmentCenter;
    )
    IF_IOS6_OR_GREATER (
      label.textAlignment = NSTextAlignmentCenter;
    )
	label.textColor = [UIColor blackColor];
	label.backgroundColor = [UIColor clearColor];
	[self.view addSubview:label];
	[label release];
	
	// create the picker to choose between UIBarButtonSystemItems
	[self createPicker];
	
	// create the label for our UIBarStyle segmented control
	//
	labelFrame = CGRectMake(0.0, 135.0, self.view.bounds.size.width, kTextFieldHeight);
	label = [[UILabel alloc] initWithFrame:labelFrame];
    label.font = [UIFont systemFontOfSize: 12];
	label.text = @"UIBarButtonSystemItem";
    IF_PRE_IOS6 (
      label.textAlignment = UITextAlignmentCenter;
    )
    IF_IOS6_OR_GREATER (
      label.textAlignment = NSTextAlignmentCenter;
    )
	label.textColor = [UIColor blackColor];
	label.backgroundColor = [UIColor clearColor];
	[self.view addSubview:label];
	[label release];
	
	// create the UIToolbar at the bottom of the view controller
	//
	toolbar = [UIToolbar new];
	toolbar.barStyle = UIBarStyleDefault;
	
	// size up the toolbar and set its frame
	[toolbar sizeToFit];
	CGFloat toolbarHeight = [toolbar frame].size.height;
	CGRect mainViewBounds = self.view.bounds;
	[toolbar setFrame:CGRectMake(CGRectGetMinX(mainViewBounds),
								 CGRectGetMinY(mainViewBounds) + CGRectGetHeight(mainViewBounds) - (toolbarHeight * 2.0) + 2.0,
								 CGRectGetWidth(mainViewBounds),
								 toolbarHeight)];
	
	[self.view addSubview:toolbar];
	
	currentSystemItem = UIBarButtonSystemItemDone;
	[self createToolbarItems];
}

- (void)toggleStyle:(id)sender
{
	UIBarButtonItemStyle style = UIBarButtonItemStylePlain;
	
	switch ([sender selectedSegmentIndex])
	{
		case 0:	// UIBarButtonItemStylePlain
		{
			style = UIBarButtonItemStylePlain;
			break;
		}
		case 1: // UIBarButtonItemStyleBordered
		{	
			style = UIBarButtonItemStyleBordered;
			break;
		}
		case 2:	// UIBarButtonItemStyleDone
		{
			style = UIBarButtonItemStyleDone;
			break;
		}
	}

	NSArray *toolbarItems = toolbar.items;
	UIBarButtonItem *item;
	for (item in toolbarItems)
	{
		item.style = style;
	}
}

- (void)toggleBarStyle:(id)sender
{
	switch ([sender selectedSegmentIndex])
	{
		case 0:
			toolbar.barStyle = UIBarStyleDefault;
			break;
		case 1:
			toolbar.barStyle = UIBarStyleBlackOpaque;
			break;
		case 2:
			toolbar.barStyle = UIBarStyleBlackTranslucent;
			break;
	}
}

- (void)action:(id)sender
{
	NSLog(@"UIBarButtonItem clicked");
}

- (void)didReceiveMemoryWarning
{
	// Invoke super's implementation to do the Right Thing, but also release the input controller since we can do that	
	// In paractice this is unlikely to be used in this application, and it would be of little benefit,
	// but the principle is the imporant thing
	//
	[super didReceiveMemoryWarning];
}


#pragma mark -
#pragma mark PickerView delegate methods

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
	// change the left most bar item to what's in the picker
	currentSystemItem = [pickerView selectedRowInComponent:0];
	[self createToolbarItems];	// this will re-create all the items
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
	return [pickerViewArray objectAtIndex:row];
}

- (CGFloat)pickerView:(UIPickerView *)pickerView widthForComponent:(NSInteger)component
{
	return 240.0;
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
	return 1;
}

@end


