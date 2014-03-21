/*

File: ButtonsViewController.m
Abstract: The view controller for hosting the UIButton features of this sample.

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

#import "ButtonsViewController.h"
#import "Constants.h"
#import "DisplayCell.h"
#import "SourceCell.h"

@implementation ButtonsViewController

@synthesize myTableView;

enum ButtonTableSections
{
	kUIGrayButton_Section = 0,
	kUIImageButton_Section,
	kUIRoundRectButton_Section,

	kUIDetailDisclosureButton_Section,
	kUIInfoLightButton_Section,
	kUIInfoDarkButton_Section,
	kUIContactAddButton_Section
};

- (id)init
{
	self = [super init];
	if (self)
	{
		// this title will appear in the navigation bar
		self.title = NSLocalizedString(@"ButtonsTitle", @"");
	}
	return self;
}

- (void)dealloc
{
	[myTableView setDelegate:nil];
	[myTableView release];
	
	[grayButton release];
	[imageButton release];
	[roundedButtonType release];
	
	[detailDisclosureButtonType release];
	[infoLightButtonType release];
	[infoDarkButtonType release];
	[contactAddButtonType release];
	
	[super dealloc];
}

+ (UIButton *)buttonWithTitle:	(NSString *)title
								target:(id)target
								selector:(SEL)selector
								frame:(CGRect)frame
								image:(UIImage *)image
								imagePressed:(UIImage *)imagePressed
								darkTextColor:(BOOL)darkTextColor
{	
	UIButton *button = [[UIButton alloc] initWithFrame:frame];
	// or you can do this:
	//		UIButton *button = [[UIButton buttonWithType:UIButtonTypeCustom] retain];
	//		button.frame = frame;
	
	button.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
	button.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
	
	[button setTitle:title forState:UIControlStateNormal];	
	if (darkTextColor)
	{
		[button setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
	}
	else
	{
		[button setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
	}
	
	UIImage *newImage = [image stretchableImageWithLeftCapWidth:12.0 topCapHeight:0.0];
	[button setBackgroundImage:newImage forState:UIControlStateNormal];
	
	UIImage *newPressedImage = [imagePressed stretchableImageWithLeftCapWidth:12.0 topCapHeight:0.0];
	[button setBackgroundImage:newPressedImage forState:UIControlStateHighlighted];
	
	[button addTarget:target action:selector forControlEvents:UIControlEventTouchUpInside];
	
    // in case the parent view draws with a custom color or gradient, use a transparent color
	button.backgroundColor = [UIColor clearColor];
		
	return button;
}

#pragma mark
#pragma mark Gray Button
#pragma mark
- (void)createGrayButton
{	
	// create the UIButtons with various background images
	// white button:
	UIImage *buttonBackground = [UIImage imageNamed:@"whiteButton.png"];
	UIImage *buttonBackgroundPressed = [UIImage imageNamed:@"blueButton.png"];
	
	CGRect frame = CGRectMake(0.0, 0.0, kStdButtonWidth, kStdButtonHeight);
	
	grayButton = [ButtonsViewController buttonWithTitle:@"Gray"
								target:self
								selector:@selector(action:)
								frame:frame
								image:buttonBackground
								imagePressed:buttonBackgroundPressed
								darkTextColor:YES];
}

#pragma mark
#pragma mark Button with Image
#pragma mark
- (void)createImageButton
{	
	// create a UIButton with just an image instead of a title
	
	UIImage *buttonBackground = [UIImage imageNamed:@"whiteButton.png"];
	UIImage *buttonBackgroundPressed = [UIImage imageNamed:@"blueButton.png"];
	
	CGRect frame = CGRectMake(0.0, 0.0, kStdButtonWidth, kStdButtonHeight);
	
	imageButton = [ButtonsViewController buttonWithTitle:@""
								target:self
								selector:@selector(action:)
								frame:frame
								image:buttonBackground
								imagePressed:buttonBackgroundPressed
								darkTextColor:YES];
	
	[imageButton setImage:[UIImage imageNamed:@"UIButton_custom.png"] forState:UIControlStateNormal];
}

#pragma mark
#pragma mark UIButtonTypeRoundedRect
#pragma mark
- (void)createRoundedButton
{
	// create a UIButton (UIButtonTypeRoundedRect)
	roundedButtonType = [[UIButton buttonWithType:UIButtonTypeRoundedRect] retain];
	roundedButtonType.frame = CGRectMake(0.0, 0.0, kStdButtonWidth, kStdButtonHeight);
	[roundedButtonType setTitle:@"Rounded" forState:UIControlStateNormal];
	roundedButtonType.backgroundColor = [UIColor clearColor];
	[roundedButtonType addTarget:self action:@selector(action:) forControlEvents:UIControlEventTouchUpInside];
}

#pragma mark
#pragma mark UIButtonTypeDetailDisclosure
#pragma mark
- (void)createDetailDisclosureButton
{
	// create a UIButton (UIButtonTypeDetailDisclosure)
	detailDisclosureButtonType = [[UIButton buttonWithType:UIButtonTypeDetailDisclosure] retain];
	detailDisclosureButtonType.frame = CGRectMake(0.0, 0.0, 25.0, 25.0);
	[detailDisclosureButtonType setTitle:@"Detail Disclosure" forState:UIControlStateNormal];
	detailDisclosureButtonType.backgroundColor = [UIColor clearColor];
	[detailDisclosureButtonType addTarget:self action:@selector(action:) forControlEvents:UIControlEventTouchUpInside];
}

#pragma mark
#pragma mark UIButtonTypeInfoDark
#pragma mark
- (void)createInfoDarkButton
{
	// create a UIButton (UIButtonTypeInfoLight)
	infoDarkButtonType = [[UIButton buttonWithType:UIButtonTypeInfoDark] retain];
	infoDarkButtonType.frame = CGRectMake(0.0, 0.0, 25.0, 25.0);
	[infoDarkButtonType setTitle:@"Detail Disclosure" forState:UIControlStateNormal];
	infoDarkButtonType.backgroundColor = [UIColor clearColor];
	[infoDarkButtonType addTarget:self action:@selector(action:) forControlEvents:UIControlEventTouchUpInside];
}

#pragma mark
#pragma mark UIButtonTypeInfoLight
#pragma mark
- (void)createInfoLightButton
{
	// create a UIButton (UIButtonTypeInfoLight)
	infoLightButtonType = [[UIButton buttonWithType:UIButtonTypeInfoLight] retain];
	infoLightButtonType.frame = CGRectMake(0.0, 0.0, 25.0, 25.0);
	[infoLightButtonType setTitle:@"Detail Disclosure" forState:UIControlStateNormal];
	infoLightButtonType.backgroundColor = [UIColor clearColor];
	[infoLightButtonType addTarget:self action:@selector(action:) forControlEvents:UIControlEventTouchUpInside];
	infoLightButtonType.backgroundColor = [UIColor grayColor];
}

#pragma mark
#pragma mark UIButtonTypeContactAdd
#pragma mark
- (void)createContactAddButton
{
	// create a UIButton (UIButtonTypeContactAdd)
	contactAddButtonType = [[UIButton buttonWithType:UIButtonTypeContactAdd] retain];
	contactAddButtonType.frame = CGRectMake(0.0, 0.0, 25.0, 25.0);
	[contactAddButtonType setTitle:@"Detail Disclosure" forState:UIControlStateNormal];
	contactAddButtonType.backgroundColor = [UIColor clearColor];
	[contactAddButtonType addTarget:self action:@selector(action:) forControlEvents:UIControlEventTouchUpInside];
}

- (void)action:(id)sender
{
	NSLog(@"UIButton was clicked");
}

- (void)loadView
{
	// create and configure the table view
	myTableView = [[UITableView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame] style:UITableViewStyleGrouped];	
	myTableView.delegate = self;
	myTableView.dataSource = self;
	myTableView.autoresizesSubviews = YES;
	self.view = myTableView;
	
	// create all our UIButtons to be used in myTableView
	[self createGrayButton];
	[self createImageButton];
	[self createRoundedButton];
	
	[self createDetailDisclosureButton];
	[self createInfoLightButton];
	[self createInfoDarkButton];
	[self createContactAddButton];
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
	return 7;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
	NSString *title;
	switch (section)
	{
		case kUIGrayButton_Section:
		{
			title = @"UIButton";
			break;
		}
		case kUIImageButton_Section:
		{
			title = @"UIButton";
			break;
		}
		case kUIRoundRectButton_Section:
		{
			title = @"UIButtonTypeRoundedRect";
			break;
		}
		case kUIDetailDisclosureButton_Section:
		{
			title = @"UIButtonTypeDetailDisclosure";
			break;
		}
		case kUIInfoLightButton_Section:
		{
			title = @"UIButtonTypeInfoLight";
			break;
		}
		case kUIInfoDarkButton_Section:
		{
			title = @"UIButtonTypeInfoDark";
			break;
		}
		case kUIContactAddButton_Section:
		{
			title = @"UIButtonTypeContactAdd";
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
		cell = [myTableView dequeueReusableCellWithIdentifier:kDisplayCell_ID];
	else if (row == 1)
		cell = [myTableView dequeueReusableCellWithIdentifier:kSourceCell_ID];
	
	if (cell == nil)
	{
        if (row == 0) {
            
            IF_PRE_IOS3
            (
             cell = [[[DisplayCell alloc] initWithFrame:CGRectZero reuseIdentifier:kDisplayCell_ID] autorelease];
             )
            IF_3_0_OR_GREATER
            (
             cell = [[[DisplayCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:kDisplayCell_ID] autorelease];
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
- (UITableViewCell *)tableView:(UITableView *)tableView 
								cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	NSInteger row = [indexPath row];
	
	UITableViewCell *cell = [self obtainTableCellForRow:row];
	
	switch (indexPath.section)
	{
		case kUIGrayButton_Section:
		{
			if (row == 0)
			{
				// this cell hosts the gray button
				((DisplayCell *)cell).nameLabel.text = @"Background Image";
				((DisplayCell *)cell).view = grayButton;
			}
			else
			{
				// this cell hosts the info on where to find the code
				((SourceCell *)cell).sourceLabel.text = @"ButtonsViewController.m - createGrayButton";
			}
			break;
		}
		
		case kUIImageButton_Section:
		{
			if (row == 0)
			{
				// this cell hosts the button with image
				((DisplayCell *)cell).nameLabel.text = @"Button with Image";
				((DisplayCell *)cell).view = imageButton;
			}
			else
			{
				// this cell hosts the info on where to find the code
				((SourceCell *)cell).sourceLabel.text = @"ButtonsViewController.m - createImageButton";
			}
			break;
		}
		
		case kUIRoundRectButton_Section:
		{
			if (row == 0)
			{
				// this cell hosts the rounded button
				((DisplayCell *)cell).nameLabel.text = @"Rounded Button";
				((DisplayCell *)cell).view = roundedButtonType;
			}
			else
			{
				// this cell hosts the info on where to find the code
				((SourceCell *)cell).sourceLabel.text = @"ButtonsViewController.m - createRoundedButton";
			}
			break;
		}

		case kUIDetailDisclosureButton_Section:
		{
			if (row == 0)
			{
				// this cell hosts the rounded button
				((DisplayCell *)cell).nameLabel.text = @"Detail Disclosure";
				((DisplayCell *)cell).view = detailDisclosureButtonType;
			}
			else
			{
				// this cell hosts the info on where to find the code
				((SourceCell *)cell).sourceLabel.text = @"ButtonsViewController.m - createDetailDisclosureButton";
			}
			break;
		}
			
		case kUIInfoLightButton_Section:
		{
			if (row == 0)
			{
				// this cell hosts the rounded button
				((DisplayCell *)cell).nameLabel.text = @"Info Light";
				((DisplayCell *)cell).view = infoLightButtonType;
			}
			else
			{
				// this cell hosts the info on where to find the code
				((SourceCell *)cell).sourceLabel.text = @"ButtonsViewController.m - createInfoLightButton";
			}
			break;
		}
		
		case kUIInfoDarkButton_Section:
		{
			if (row == 0)
			{
				// this cell hosts the rounded button
				((DisplayCell *)cell).nameLabel.text = @"Info Dark";
				((DisplayCell *)cell).view = infoDarkButtonType;
			}
			else
			{
				// this cell hosts the info on where to find the code
				((SourceCell *)cell).sourceLabel.text = @"ButtonsViewController.m - createInfoDarkButton";
			}
			break;
		}
			
		case kUIContactAddButton_Section:
		{
			if (row == 0)
			{
				// this cell hosts the rounded button
				((DisplayCell *)cell).nameLabel.text = @"Contact Add";
				((DisplayCell *)cell).view = contactAddButtonType;
			}
			else
			{
				// this cell hosts the info on where to find the code
				((SourceCell *)cell).sourceLabel.text = @"ButtonsViewController.m - createContactAddButton";
			}
			break;
		}		
	}
	
	return cell;
}

@end

