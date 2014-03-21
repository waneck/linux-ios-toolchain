/*

File: TextViewController.m
Abstract: The view controller for hosting the UITextView features of this
sample.

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

#import "TextViewController.h"
#import "CellTextView.h"
#import "SourceCell.h"
#import "Constants.h"


@implementation TextViewController

#define kUITextViewCellRowHeight 150.0

@synthesize myTableView;

- (id)init
{
	self = [super init];
	if (self)
	{
		// this title will appear in the navigation bar
		self.title = NSLocalizedString(@"TextViewTitle", @"");
	}
	
	return self;
}

- (void)dealloc
{
	[myTableView release];
	
	[super dealloc];
}

- (void)loadView
{
	// create and configure the table view
	myTableView = [[UITableView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame] style:UITableViewStyleGrouped];	
	myTableView.delegate = self;
	myTableView.dataSource = self;
	myTableView.scrollEnabled = NO; // no scrolling in this case, we don't want to interfere with text view scrolling
	myTableView.autoresizesSubviews = YES;
	
	self.view = myTableView;
}

- (UITextView *)create_UITextView
{
	CGRect frame = CGRectMake(0.0, 0.0, 100.0, 100.0);
   
	UITextView *textView = [[[UITextView alloc] initWithFrame:frame] autorelease];
    textView.textColor = [UIColor blackColor];
    textView.font = [UIFont fontWithName:kFontName size:kTextViewFontSize];
    textView.delegate = self;
    textView.backgroundColor = [UIColor whiteColor];
	
	textView.text = @"Now is the time for all good developers to come to serve their country.\n\nNow is the time for all good developers to come to serve their country.";
	textView.returnKeyType = UIReturnKeyDefault;
	textView.keyboardType = UIKeyboardTypeDefault;	// use the default type input method (entire keyboard)
	
	// note: for UITextView, if you don't like autocompletion while typing use:
	// myTextView.autocorrectionType = UITextAutocorrectionTypeNo;
	
	return textView;
}


#pragma mark UITextView delegate methods

- (void)textViewDidBeginEditing:(UITextView *)textView
{
	// provide my own Save button to dismiss the keyboard
	UIBarButtonItem* saveItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone
															target:self action:@selector(saveAction:)];
	self.navigationItem.rightBarButtonItem = saveItem;
	[saveItem release];
}

- (void)saveAction:(id)sender
{
	// finish typing text/dismiss the keyboard by removing it as the first responder
	//
	UITableViewCell *cell = [myTableView cellForRowAtIndexPath:[NSIndexPath indexPathForRow:0 inSection:0]];

	[((CellTextView *)cell).view resignFirstResponder];
	self.navigationItem.rightBarButtonItem = nil;	// this will remove the "save" button
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
	return 1;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
	return @"UITextView";
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
			result = kUITextViewCellRowHeight;
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
		cell = [myTableView dequeueReusableCellWithIdentifier:kCellTextView_ID];
	else if (row == 1)
		cell = [myTableView dequeueReusableCellWithIdentifier:kSourceCell_ID];
		
	if (cell == nil)
	{
        if (row == 0) {
            
            IF_PRE_IOS3
            (
             cell = [[[CellTextView alloc] initWithFrame:CGRectZero reuseIdentifier:kCellTextView_ID] autorelease];
             )
            IF_3_0_OR_GREATER
            (
             cell = [[[CellTextView alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:kCellTextView_ID] autorelease];
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
        }
	}
	
	return cell;
}

// to determine which UITableViewCell to be used on a given row.
//
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	NSInteger row = [indexPath row];
	UITableViewCell *sourceCell = [self obtainTableCellForRow:row];
	
	// we are creating a new cell, setup its attributes
	if (row == 0)
	{
		// this cell hosts the UISwitch control
		((CellTextView *)sourceCell).view = [self create_UITextView];
	}
	else
	{
		// this cell hosts the info on where to find the code
		((SourceCell *)sourceCell).sourceLabel.text = @"TextViewController.m - create_UITextView";
	}
	
	return sourceCell;
}

@end

