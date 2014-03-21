/*

File: TextFieldController.m
Abstract: The view controller for hosting the UITextField features of this
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

#import "TextFieldController.h"
#import "Constants.h"
#import "CellTextField.h"
#import "SourceCell.h"

// the amount of vertical shift upwards keep the text field in view as the keyboard appears
#define kOFFSET_FOR_KEYBOARD					150.0

#define kTextFieldWidth							100.0	// initial width, but the table cell will dictact the actual width

// the duration of the animation for the view shift
#define kVerticalOffsetAnimationDuration		0.30

#define kUITextField_Section					0
#define kUITextField_Rounded_Custom_Section		1
#define kUITextField_Secure_Section				2

// Private interface for TextFieldController - internal only methods.
@interface TextFieldController (Private)
- (void)setViewMovedUp:(BOOL)movedUp;
@end

@implementation TextFieldController

@synthesize myTableView;

- (id)init
{
	self = [super init];
	if (self)
	{
		// this title will appear in the navigation bar
		self.title = NSLocalizedString(@"TextFieldTitle", @"");
	}
	
	return self;
}

- (void)dealloc
{
	[myTableView release];
	[textField release];
	[textFieldRounded release];
	[textFieldSecure release];
	
	[super dealloc];
}

#pragma mark
#pragma mark UITextField
#pragma mark
- (UITextField *)createTextField
{
	CGRect frame = CGRectMake(0.0, 0.0, kTextFieldWidth, kTextFieldHeight);
	UITextField *returnTextField = [[UITextField alloc] initWithFrame:frame];
   
	returnTextField.borderStyle = UITextBorderStyleBezel;
    returnTextField.textColor = [UIColor blackColor];
	returnTextField.font = [UIFont systemFontOfSize:17.0];
    returnTextField.placeholder = @"<enter text>";
    returnTextField.backgroundColor = [UIColor whiteColor];
	returnTextField.autocorrectionType = UITextAutocorrectionTypeNo;	// no auto correction support
	
	returnTextField.keyboardType = UIKeyboardTypeDefault;	// use the default type input method (entire keyboard)
	returnTextField.returnKeyType = UIReturnKeyDone;
	
	returnTextField.clearButtonMode = UITextFieldViewModeWhileEditing;	// has a clear 'x' button to the right
	
	return returnTextField;
}

#pragma mark
#pragma mark UITextField - rounded
#pragma mark
- (UITextField *)createTextField_Rounded
{
	CGRect frame = CGRectMake(0.0, 0.0, kTextFieldWidth, kTextFieldHeight);
	UITextField *returnTextField = [[UITextField alloc] initWithFrame:frame];
    
	returnTextField.borderStyle = UITextBorderStyleRoundedRect;
    returnTextField.textColor = [UIColor blackColor];
	returnTextField.font = [UIFont systemFontOfSize:17.0];
    returnTextField.placeholder = @"<enter text>";
    returnTextField.backgroundColor = [UIColor whiteColor];
	returnTextField.autocorrectionType = UITextAutocorrectionTypeNo;	// no auto correction support
	
	returnTextField.keyboardType = UIKeyboardTypeDefault;
	returnTextField.returnKeyType = UIReturnKeyDone;
	
	returnTextField.clearButtonMode = UITextFieldViewModeWhileEditing;	// has a clear 'x' button to the right
	
	return returnTextField;
}

#pragma mark
#pragma mark UITextField - secure
#pragma mark
- (UITextField *)createTextField_Secure
{
	CGRect frame = CGRectMake(0.0, 0.0, kTextFieldWidth, kTextFieldHeight);
	UITextField *returnTextField = [[UITextField alloc] initWithFrame:frame];
    returnTextField.borderStyle = UITextBorderStyleBezel;
    returnTextField.textColor = [UIColor blackColor];
	returnTextField.font = [UIFont systemFontOfSize:17.0];
    returnTextField.placeholder = @"<enter password>";
    returnTextField.backgroundColor = [UIColor whiteColor];
	
	returnTextField.keyboardType = UIKeyboardTypeDefault;
	returnTextField.returnKeyType = UIReturnKeyDone;	
	returnTextField.secureTextEntry = YES;	// make the text entry secure (bullets)
	
	returnTextField.clearButtonMode = UITextFieldViewModeWhileEditing;	// has a clear 'x' button to the right
	
	return returnTextField;
}

- (void)loadView
{
	// setup our parent content view and embed it to your view controller
	UIView *contentView = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
	contentView.backgroundColor = [UIColor blackColor];
	contentView.autoresizesSubviews = YES;
	self.view = contentView;
	[contentView release];
	
	self.editing = NO;
	self.navigationItem.rightBarButtonItem = self.editButtonItem;
	
	// create and configure the table view
	myTableView = [[UITableView alloc] initWithFrame:self.view.bounds style:UITableViewStyleGrouped];	
	myTableView.delegate = self;
	myTableView.dataSource = self;

	myTableView.scrollEnabled = NO;	// no scrolling in this case, we don't want to interfere with touch events on edit fields
	[self.view addSubview: myTableView];
	
	// create our text fields to be recycled when UITableViewCells are created
	textField = [self createTextField];	
	textFieldRounded = [self createTextField_Rounded];	
	textFieldSecure = [self createTextField_Secure];
	
	// create the UIToolbar at the bottom of the view controller
	//
	UIToolbar *toolbar = [UIToolbar new];
	toolbar.barStyle = UIBarStyleDefault;
	
	CGRect frame = CGRectMake(0.0, 0.0, kSwitchButtonWidth, kSwitchButtonHeight);
	UISwitch *switchCtl = [[UISwitch alloc] initWithFrame:frame];
	[switchCtl addTarget:self action:@selector(leftViewAction:) forControlEvents:UIControlEventValueChanged];
	switchCtl.backgroundColor = [UIColor clearColor];
	UIBarButtonItem *customItem = [[UIBarButtonItem alloc] initWithCustomView:switchCtl];
	[switchCtl release];
	
	frame = CGRectMake(0.0, 0.0, 70.0, kLabelHeight);
	UILabel *label = [[[UILabel alloc] initWithFrame:frame] autorelease];
	
    
    IF_PRE_IOS6 (
     label.textAlignment = UITextAlignmentRight;
    )
    IF_IOS6_OR_GREATER (
     label.textAlignment = NSTextAlignmentRight;
    )
    label.text = NSLocalizedString(@"LeftView", @"");
    label.font = [UIFont boldSystemFontOfSize:14.0];
    label.backgroundColor = [UIColor clearColor];
	label.textColor = [UIColor colorWithRed:76.0/255.0 green:86.0/255.0 blue:108.0/255.0 alpha:1.0];
	UIBarButtonItem *labelItem = [[UIBarButtonItem alloc] initWithCustomView:label];
	[label release];
		
	NSArray *items = [NSArray arrayWithObjects: labelItem, customItem, nil];
	toolbar.items = items;
	[customItem release];
	[labelItem release];
								   
	// size up the toolbar and set its frame
	[toolbar sizeToFit];
	CGFloat toolbarHeight = [toolbar frame].size.height;
	CGRect mainViewBounds = self.view.bounds;
	[toolbar setFrame:CGRectMake(CGRectGetMinX(mainViewBounds),
								 CGRectGetMinY(mainViewBounds) + CGRectGetHeight(mainViewBounds) - (toolbarHeight * 2.0),
								 CGRectGetWidth(mainViewBounds),
								 toolbarHeight)];
	
	[self.view addSubview:toolbar];
	[toolbar release];	
}

// affects the leftView property of UITextField, essentially adding any UIView you want.
// note also you can do the same for the right by affecting its "rightView" property.
//
- (void)leftViewAction:(id)sender
{
	UISwitch *switchCtl = (UISwitch*)sender;
	if (switchCtl.on)
	{
		// add the image to each text field
		textField.leftView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"segment_check.png"]];
		textField.leftViewMode = UITextFieldViewModeAlways;
		
		textFieldRounded.leftView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"segment_check.png"]];
		textFieldRounded.leftViewMode = UITextFieldViewModeAlways;
		
		textFieldSecure.leftView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"segment_check.png"]];
		textFieldSecure.leftViewMode = UITextFieldViewModeAlways;
	}
	else
	{
		// remove the leftView image from each text field
		textField.leftView = nil;
		textFieldRounded.leftView = nil;
		textFieldSecure.leftView = nil;
	}
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
	return 3;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
	NSString *title;
	switch (section)
	{
		case kUITextField_Section:
		{
			title = @"UITextField";
			break;
		}
		case kUITextField_Rounded_Custom_Section:
		{
			title = @"UITextField Rounded";
			break;
		}
		case kUITextField_Secure_Section:
		{
			title = @"UITextField Secure";
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

// utility routine leveraged by 'cellForRowAtIndexPath' to determine which UITableViewCell to be used on a given row
//
- (UITableViewCell *)obtainTableCellForRow:(NSInteger)row
{
	UITableViewCell *cell = nil;

	if (row == 0)
		cell = [myTableView dequeueReusableCellWithIdentifier:kCellTextField_ID];
	else if (row == 1)
		cell = [myTableView dequeueReusableCellWithIdentifier:kSourceCell_ID];
	
	if (cell == nil)
	{
        if (row == 0) {
            
            IF_PRE_IOS3
            (
             cell = [[[CellTextField alloc] initWithFrame:CGRectZero reuseIdentifier:kCellTextField_ID] autorelease];
             )
            IF_3_0_OR_GREATER
            (
             cell = [[[CellTextField alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:kCellTextField_ID] autorelease];
             )
			((CellTextField *)cell).delegate = self;	// so we can detect when cell editing starts
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
	
	switch (indexPath.section)
	{
		case kUITextField_Section:
		{
			if (row == 0)
			{
				// this cell hosts the text field control
				((CellTextField *)sourceCell).view = textField;
			}
			else
			{	
				// this cell hosts the info on where to find the code
				((SourceCell *)sourceCell).sourceLabel.text = @"TextFieldController.m - createTextField";
			}
			textFieldCell = (CellTextField *)sourceCell;	// kept track for editing
			break;
		}
		
		case kUITextField_Rounded_Custom_Section:
		{
			if (row == 0)
			{
				// this cell hosts the rounded text field control
				((CellTextField *)sourceCell).view = textFieldRounded;
			}
			else
			{
				// this cell hosts the info on where to find the code
				((SourceCell *)sourceCell).sourceLabel.text = @"TextFieldController.m - createTextField_Rounded";
			}
			textFieldRoundedCell = (CellTextField *)sourceCell;	// kept track for editing
			break;	
		}
		
		case kUITextField_Secure_Section:
		{
			// we are creating a new cell, setup its attributes
			if (row == 0)
			{
				// this cell hosts the secure text field control
				((CellTextField *)sourceCell).view = textFieldSecure;
			}
			else
			{
				// this cell hosts the info on where to find the code
				((SourceCell *)sourceCell).sourceLabel.text = @"TextFieldController.m - createTextField_Secure";
			}
			textFieldSecureCell = (CellTextField *)sourceCell;	// kept track for editing
			break;
		}
	}
	
    return sourceCell;
}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
	return 37.0;
}


#pragma mark -
#pragma mark <EditableTableViewCellDelegate> Methods and editing management

- (BOOL)cellShouldBeginEditing:(EditableTableViewCell *)cell
{
    // notify other cells to end editing
    if (![cell isEqual:textFieldCell])
		[textFieldCell stopEditing];
    if (![cell isEqual:textFieldRoundedCell])
		[textFieldRoundedCell stopEditing];
    if (![cell isEqual:textFieldSecureCell])
		[textFieldSecureCell stopEditing];
		
    return self.editing;
}

- (void)cellDidEndEditing:(EditableTableViewCell *)cell
{
	if ([cell isEqual:textFieldSecureCell] || [cell isEqual:textFieldRoundedCell])
	{
        // Restore the position of the main view if it was animated to make room for the keyboard.
        if  (self.view.frame.origin.y < 0)
		{
            [self setViewMovedUp:NO];
        }
    }
}

// Animate the entire view up or down, to prevent the keyboard from covering the author field.
- (void)setViewMovedUp:(BOOL)movedUp
{
    [UIView beginAnimations:nil context:NULL];
    [UIView setAnimationDuration:0.3];
    // Make changes to the view's frame inside the animation block. They will be animated instead
    // of taking place immediately.
    CGRect rect = self.view.frame;
    if (movedUp)
	{
        // If moving up, not only decrease the origin but increase the height so the view 
        // covers the entire screen behind the keyboard.
        rect.origin.y -= kOFFSET_FOR_KEYBOARD;
        rect.size.height += kOFFSET_FOR_KEYBOARD;
    }
	else
	{
        // If moving down, not only increase the origin but decrease the height.
        rect.origin.y += kOFFSET_FOR_KEYBOARD;
        rect.size.height -= kOFFSET_FOR_KEYBOARD;
    }
    self.view.frame = rect;
    
    [UIView commitAnimations];
}

- (void)setEditing:(BOOL)editing animated:(BOOL)animated
{
    [super setEditing:editing animated:animated];
    if (!editing)
	{
        [textFieldCell stopEditing];
        [textFieldRoundedCell stopEditing];
        [textFieldSecureCell stopEditing];
    }
}

- (void)keyboardWillShow:(NSNotification *)notif
{
    // The keyboard will be shown. If the user is editing the author, adjust the display so that the
    // author field will not be covered by the keyboard.
    if ((textFieldRoundedCell.isInlineEditing || textFieldSecureCell.isInlineEditing) && self.view.frame.origin.y >= 0)
	{
        [self setViewMovedUp:YES];
    }
	else if (!textFieldSecureCell.isInlineEditing && self.view.frame.origin.y < 0)
	{
        [self setViewMovedUp:NO];
    }
}


#pragma mark - UIViewController delegate methods

- (void)viewWillAppear:(BOOL)animated
{
    // watch the keyboard so we can adjust the user interface if necessary.
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillShow:) 
            name:UIKeyboardWillShowNotification object:self.view.window]; 
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self setEditing:NO animated:YES];
	
    // unregister for keyboard notifications while not visible.
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIKeyboardWillShowNotification object:nil]; 
}

@end

