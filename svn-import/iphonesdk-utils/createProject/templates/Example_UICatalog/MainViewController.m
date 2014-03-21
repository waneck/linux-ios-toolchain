/*

File: MainViewController.m
Abstract: The application's main view controller (front page).

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

#import "MainViewController.h"

#import "ButtonsViewController.h"
#import "ControlsViewController.h"
#import "TextFieldController.h"
#import "SearchBarController.h"
#import "TextViewController.h"
#import "SegmentViewController.h"
#import "ToolbarViewController.h"
#import "PickerViewController.h"
#import "ImagesViewController.h"
#import "WebViewController.h"
#import "AlertsViewController.h"
#import "TransitionViewController.h"

#import "Constants.h"

static NSString *kCellIdentifier = @"MyIdentifier";

@implementation MainViewController

@synthesize myTableView;

- (id)init
{
	if (self = [super init])
	{		
		// this title will appear in the navigation bar
		self.title = NSLocalizedString(@"UICatalogTitle", @"");
		
		// construct the array of page descriptions we will use (each desc is a dictionary)
		//
		menuList = [[NSMutableArray alloc] init];
	}
	return self;
}

-(void)loadView
{	
	// setup our parent content view and embed it this view controller
	// having a generic contentView (as opposed to UITableView taking up the entire view controller)
	// makes your UI design more flexible as you can add more subviews later
	//
	UIView *contentView = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
	self.view = contentView;
	[contentView release];
	
	self.view.autoresizesSubviews = YES;
	
	// for showing various UIButtons:
	ButtonsViewController *buttonsViewController = [[ButtonsViewController alloc] init];
	[menuList addObject:[NSDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"ButtonsTitle", @""), @"title",
						 buttonsViewController, @"viewController",
						 nil]];
	[buttonsViewController release];
	
	// for showing various UIControls:
	ControlsViewController *controlsViewController = [[ControlsViewController alloc] init];
	[menuList addObject:[NSDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"ControlsTitle", @""), @"title",
						 controlsViewController, @"viewController",
						 nil]];
	[controlsViewController release];
	
	// for showing various UITextFields:
	TextFieldController *textFieldViewController = [[TextFieldController alloc] init];
	[menuList addObject:[NSDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"TextFieldTitle", @""), @"title",
						 textFieldViewController, @"viewController",
						 nil]];
	[textFieldViewController release];
	
	// for showing various UITextFields:
	SearchBarController *searchBarController = [[SearchBarController alloc] init];
	[menuList addObject:[NSDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"SearchBarTitle", @""), @"title",
						 searchBarController, @"viewController",
						 nil]];
	[searchBarController release];
	
	// for showing UITextView:
	TextViewController *textViewController = [[TextViewController alloc] init];
	[menuList addObject:[NSDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"TextViewTitle", @""), @"title",
						 textViewController, @"viewController",
						 nil]];
	[textViewController release];
	
	// for showing various UIPickers:
	PickerViewController *pickerViewController = [[PickerViewController alloc] init];
	[menuList addObject:[NSDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"PickerTitle", @""), @"title",
						 pickerViewController, @"viewController",
						 nil]];
	[pickerViewController release];
	
	// for showing UIImageView:
	ImagesViewController *imagesViewController = [[ImagesViewController alloc] init];
	[menuList addObject:[NSDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"ImagesTitle", @""), @"title",
						 imagesViewController, @"viewController",
						 nil]];
	[imagesViewController release];	
	
	// for showing UIWebView:
	WebViewController *webViewController = [[WebViewController alloc] init];
	[menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"WebTitle", @""), @"title",
						 webViewController, @"viewController",
						 nil]];
	[webViewController release];	
	
	// for showing various UISegmentedControls:
	SegmentViewController *segmentViewController = [[SegmentViewController alloc] init];
	[menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"SegmentTitle", @""), @"title",
						 segmentViewController, @"viewController",
						 nil]];
	[segmentViewController release];
	
	// for showing various UIBarButtonItem items inside a UIToolbar:
	ToolbarViewController *toolbarViewController = [[ToolbarViewController alloc] init];
	[menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"ToolbarTitle", @""), @"title",
						 toolbarViewController, @"viewController",
						 nil]];
	[toolbarViewController release];
	
	// for showing various UIActionSheets and UIAlertViews:
	AlertsViewController *alertsViewController = [[AlertsViewController alloc] init];
	[menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"AlertTitle", @""), @"title",
						 alertsViewController, @"viewController",
						 nil]];
	[alertsViewController release];
	
	// for showing how to a use flip animation transition between two UIViews:
	TransitionsViewController *transitionsViewController = [[TransitionsViewController alloc] init];
	[menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
						 NSLocalizedString(@"TransitionsTitle", @""), @"title",
						 transitionsViewController, @"viewController",
						 nil]];
	[transitionsViewController release];
	
	// create a custom navigation bar button and set it to always say "Back"
	UIBarButtonItem *temporaryBarButtonItem = [[UIBarButtonItem alloc] init];
	temporaryBarButtonItem.title = @"Back";
	self.navigationItem.backBarButtonItem = temporaryBarButtonItem;
	[temporaryBarButtonItem release];

	// finally create a our table, its contents will be populated by "menuList" using the UITableView delegate methods
	myTableView = [[UITableView alloc] initWithFrame:self.view.bounds style:UITableViewStylePlain];
	myTableView.delegate = self;
	myTableView.dataSource = self;
	myTableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
	
	// setup our list view to autoresizing in case we decide to support autorotation along the other UViewControllers
	myTableView.autoresizesSubviews = YES;
	myTableView.autoresizingMask = (UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight);
	
	[myTableView reloadData];	// populate our table's data
	[self.view addSubview: myTableView];
}

- (void)dealloc
{
	[menuList release];
	[myTableView release];
	
	[super dealloc];
}


#pragma mark UIViewController delegate methods

- (void)viewWillAppear:(BOOL)animated
{
	// this UIViewController is about to re-appear, make sure we remove the current selection in our table view
	NSIndexPath *tableSelection = [myTableView indexPathForSelectedRow];
	[myTableView deselectRowAtIndexPath:tableSelection animated:NO];
}


#pragma mark UITableView delegate methods

IF_PRE_IOS3(
// decide what kind of accesory view (to the far right) we will use
- (UITableViewCellAccessoryType)tableView:(UITableView *)tableView accessoryTypeForRowWithIndexPath:(NSIndexPath *)indexPath
{
	return UITableViewCellAccessoryDisclosureIndicator;
}
)

// tell our table how many sections or groups it will have (always 1 in our case)
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	return 1;
}

// the table's selection has changed, switch to that item's UIViewController
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	UIViewController *targetViewController = [[menuList objectAtIndex: indexPath.row] objectForKey:@"viewController"];
	[[self navigationController] pushViewController:targetViewController animated:YES];
}


#pragma mark UITableView datasource methods

// tell our table how many rows it will have, in our case the size of our menuList
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	return [menuList count];
}

// tell our table what kind of cell to use and its title for the given row
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:kCellIdentifier];
	if (cell == nil)
	{
        IF_PRE_IOS3
        (
		   cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:kCellIdentifier] autorelease];
        )
        IF_3_0_OR_GREATER
        (
            cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:kCellIdentifier] autorelease];
        )
	}
	
	IF_PRE_IOS3
	(
		cell.text = [[menuList objectAtIndex:indexPath.row] objectForKey:@"title"];
	)
	IF_3_0_OR_GREATER
	(
		cell.textLabel.text = [[menuList objectAtIndex:indexPath.row] objectForKey:@"title"];
	)
	
    IF_3_0_OR_GREATER
	(
        cell.accessoryType =  UITableViewCellAccessoryDisclosureIndicator;
    )

	return cell;
}

@end


