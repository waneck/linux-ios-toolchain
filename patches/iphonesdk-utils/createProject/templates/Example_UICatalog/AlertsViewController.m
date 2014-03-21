/*

File: AlertsViewController.m
Abstract: The view controller for hosting various kinds of alerts and action
sheets.

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

#import "AlertsViewController.h"
#import "SourceCell.h"
#import "Constants.h"

@implementation AlertsViewController

NSString *kAlertCell_ID = @"AlertCell_ID";

enum AlertTableSections
{
	kUIAction_Simple_Section = 0,
	kUIAction_OKCancel_Section,
	kUIAction_Custom_Section,
	kUIAlert_Simple_Section,
	kUIAlert_OKCancel_Section,
	kUIAlert_Custom_Section,
};

@synthesize myTableView;

- (id)init
{
	self = [super init];
	if (self)
	{
		// this title will appear in the navigation bar
		self.title = NSLocalizedString(@"AlertTitle", @"");
	}
	return self;
}

- (void)loadView
{
	// create and configure the table view
	myTableView = [[UITableView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame] style:UITableViewStyleGrouped];	
	myTableView.delegate = self;
	myTableView.dataSource = self;
	myTableView.autoresizesSubviews = YES;
	
	self.view = myTableView;
}

#pragma mark UIActionSheet

- (void)dialogSimpleAction
{
	// open a dialog with just an OK button
	UIActionSheet *actionSheet = [[UIActionSheet alloc] initWithTitle:@"UIActionSheet <title>"
									delegate:self cancelButtonTitle:nil destructiveButtonTitle:@"OK" otherButtonTitles:nil];
	actionSheet.actionSheetStyle = UIActionSheetStyleDefault;
	[actionSheet showInView:self.view];	// show from our table view (pops up in the middle of the table)
	[actionSheet release];
}

- (void)dialogOKCancelAction
{
	// open a dialog with an OK and cancel button
	UIActionSheet *actionSheet = [[UIActionSheet alloc] initWithTitle:@"UIActionSheet <title>"
									delegate:self cancelButtonTitle:@"Cancel" destructiveButtonTitle:@"OK" otherButtonTitles:nil];
	actionSheet.actionSheetStyle = UIActionSheetStyleDefault;
	[actionSheet showInView:self.view]; // show from our table view (pops up in the middle of the table)
	[actionSheet release];
}

- (void)dialogOtherAction
{
	// open a dialog with two custom buttons
	UIActionSheet *actionSheet = [[UIActionSheet alloc] initWithTitle:@"UIActionSheet <title>"
									delegate:self cancelButtonTitle:nil destructiveButtonTitle:nil
									otherButtonTitles:@"Button1", @"Button2", nil];
	actionSheet.actionSheetStyle = UIActionSheetStyleDefault;
	actionSheet.destructiveButtonIndex = 1;	// make the second button red (destructive)
	[actionSheet showInView:self.view]; // show from our table view (pops up in the middle of the table)
	[actionSheet release];
}


#pragma mark UIAlertView

- (void)alertSimpleAction
{
	// open an alert with just an OK button
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"UIAlertView" message:@"<Alert message>"
							delegate:self cancelButtonTitle:@"OK" otherButtonTitles: nil];
	[alert show];	
	[alert release];
}

- (void)alertOKCancelAction
{
	// open a alert with an OK and cancel button
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"UIAlertView" message:@"<Alert message>"
							delegate:self cancelButtonTitle:@"Cancel" otherButtonTitles:@"OK", nil];
	[alert show];
	[alert release];
}

- (void)alertOtherAction
{
	// open an alert with two custom buttons
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"UIAlertView" message:@"<Alert message>"
							delegate:self cancelButtonTitle:@"Cancel" otherButtonTitles:@"Button1", @"Button2", nil];
	[alert show];
	[alert release];
}


#pragma mark - UIActionSheetDelegate

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
	// the user clicked one of the OK/Cancel buttons
	if (buttonIndex == 0)
	{
		NSLog(@"ok");
	}
	else
	{
		NSLog(@"cancel");
	}
	
	[myTableView deselectRowAtIndexPath:[myTableView indexPathForSelectedRow] animated:NO];
}

#pragma mark - UIAlertViewDelegate

- (void)alertView:(UIAlertView *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
	// use "buttonIndex" to decide your action
	//
	
	[myTableView deselectRowAtIndexPath:[myTableView indexPathForSelectedRow] animated:NO];
}

#pragma mark - UITableView delegates

// if you want the entire table to just be re-orderable then just return UITableViewCellEditingStyleNone
//
- (UITableViewCellEditingStyle)tableView:(UITableView *)tableView editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath
{
	return UITableViewCellEditingStyleNone;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	return 6;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
	NSString *title;
	switch (section)
	{
		case kUIAction_Simple_Section:
		{
			title = @"UIActionSheet";
			break;
		}
		case kUIAction_OKCancel_Section:
		{
			title = @"UIActionSheet";
			break;
		}
		case kUIAction_Custom_Section:
		{
			title = @"UIActionSheet";
			break;
		}
		case kUIAlert_Simple_Section:
		{
			title = @"UIAlertView";
			break;
		}
		case kUIAlert_OKCancel_Section:
		{
			title = @"UIAlertView";
			break;
		}
		case kUIAlert_Custom_Section:
		{
			title = @"UIAlertView";
			break;
		}

	}
	return title;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	return 2;
}

// to determine specific row height for each cell, override this.  In this example, each row is determined
// buy the its subviews that are embedded.
//
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
	CGFloat result;
	
	switch ([indexPath row])
	{
		case 0:
		{
			result = kUIRowHeight;
			break;
		}
		case 1:
		{
			result = kUIRowLabelHeight;
			break;
		}
	}

	return result;
}

// utility routine leveraged by 'cellForRowAtIndexPath' to determine which UITableViewCell to be used on a given row.
//
- (UITableViewCell *)obtainTableCellForRow:(NSInteger)row
{
	UITableViewCell *cell = nil;
	
	if (row == 0)
		cell = [myTableView dequeueReusableCellWithIdentifier:kAlertCell_ID];
	else if (row == 1)
		cell = [myTableView dequeueReusableCellWithIdentifier:kSourceCell_ID];

	if (cell == nil)
	{
        if (row == 0) {
            
            IF_PRE_IOS3
            (
             cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:kAlertCell_ID] autorelease];
             )
            IF_3_0_OR_GREATER
            (
             cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:kAlertCell_ID] autorelease];
             )
        }
        
		else if (row == 1) {
            IF_PRE_IOS3
            (
             cell = [[[SourceCell alloc] initWithFrame:CGRectZero reuseIdentifier:kSourceCell_ID] autorelease];
             )
            IF_3_0_OR_GREATER
            (
             cell = [[[SourceCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:kSourceCell_ID] autorelease];
             )
			cell.selectionStyle = UITableViewCellSelectionStyleNone;
        }

	}
	
	return cell;
}

// the table's selection has changed, show the alert or action sheet
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	if (indexPath.row == 0)
	{
		switch (indexPath.section)
		{
			case kUIAction_Simple_Section:
			{
				[self dialogSimpleAction];
				break;
			}
				
			case kUIAction_OKCancel_Section:
			{
				[self dialogOKCancelAction];
				break;
			}
				
			case kUIAction_Custom_Section:
			{
				[self dialogOtherAction];
				break;
			}
				
			case kUIAlert_Simple_Section:
			{
				[self alertSimpleAction];
				break;
			}
				
			case kUIAlert_OKCancel_Section:
			{
				[self alertOKCancelAction];	
				break;
			}
				
			case kUIAlert_Custom_Section:
			{
				[self alertOtherAction];	
				break;
			}
		}
	}
}

// to determine which UITableViewCell to be used on a given row.
//
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	NSInteger row = [indexPath row];
	UITableViewCell *cell = [self obtainTableCellForRow:row];
	
	switch (indexPath.section)
	{
		case kUIAction_Simple_Section:
		{
			if (row == 0)
			{
				IF_PRE_IOS3
				(
				    cell.text = @"Show Simple";
				)
				IF_3_0_OR_GREATER
				(
				    cell.textLabel.text = @"Show Simple";
				)
			}
			else
			{
				// this cell hosts the info on where to find the code
				((SourceCell *)cell).sourceLabel.text = @"AlertsViewController.m - dialogSimpleAction";
			}
			break;
		}
		
		case kUIAction_OKCancel_Section:
		{
			if (row == 0)
			{
				IF_PRE_IOS3
				(
				    cell.text = @"Show OK-Cancel";
				)
				IF_3_0_OR_GREATER
				(
				    cell.textLabel.text = @"Show OK-Cancel";
				)
			}
			else
			{
				// this cell hosts the info on where to find the code
				((SourceCell *)cell).sourceLabel.text = @"AlertsViewController.m - dialogOKCancelAction";
			}
			break;
		}
		
		case kUIAction_Custom_Section:
		{
			if (row == 0)
			{
				IF_PRE_IOS3
				(
				    cell.text = @"Show Customized";
				)
				IF_3_0_OR_GREATER
				(
				    cell.textLabel.text = @"Show Customized";
				)
			}
			else
			{
				// this cell hosts the info on where to find the code
				((SourceCell *)cell).sourceLabel.text = @"AlertsViewController.m - dialogOtherAction";
			}
			break;
		}
		
		case kUIAlert_Simple_Section:
		{
			if (row == 0)
			{
				IF_PRE_IOS3
				(
				    cell.text = @"Show Simple";
				)
				IF_3_0_OR_GREATER
				(
				    cell.textLabel.text = @"Show Simple";
				)
			}
			else
			{
				// this cell hosts the info on where to find the code
				((SourceCell *)cell).sourceLabel.text = @"AlertsViewController.m - alertSimpleAction";
			}
			break;
		}
		
		case kUIAlert_OKCancel_Section:
		{
			if (row == 0)
			{
				IF_PRE_IOS3
				(
				    cell.text = @"Show OK-Cancel";
				)
				IF_3_0_OR_GREATER
				(
				    cell.textLabel.text = @"Show OK-Cancel";
				)
			}						
			else
			{
				// this cell hosts the info on where to find the code
				((SourceCell *)cell).sourceLabel.text = @"AlertsViewController.m - alertOKCancelAction";
			}
			break;
		}
		
		case kUIAlert_Custom_Section:
		{
			if (row == 0)
			{
				IF_PRE_IOS3
				(
				    cell.text = @"Show Customized";
				)
				IF_3_0_OR_GREATER
				(
				    cell.textLabel.text = @"Show Customized";
				)
			}						
			else
			{
				// this cell hosts the info on where to find the code
				((SourceCell *)cell).sourceLabel.text = @"AlertsViewController.m - alertOtherAction";
			}
			break;
		}
	}
	
	return cell;
}

@end

