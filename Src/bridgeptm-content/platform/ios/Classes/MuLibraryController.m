//
//  MuLibraryController.m
//  MuPDF
//
//  Copyright (c) 2013 Artifex Software, Inc. All rights reserved.
//

#include "common.h"
#import "MuDocumentController.h"
#import "MuLibraryController.h"

static void showAlert(NSString *msg, NSString *filename)
{
	UIAlertView *alert = [[UIAlertView alloc]
		initWithTitle: msg
		message: filename
		delegate: nil
		cancelButtonTitle: @"Okay"
		otherButtonTitles: nil];
	[alert show];
}

@implementation MuLibraryController

- (void) viewWillAppear: (BOOL)animated
{

    [self.navigationController setNavigationBarHidden:YES];

	[self setTitle: @"PDF documents"];

	[self reload];
	printf("library viewWillAppear (starting reload timer)\n");
	timer = [NSTimer timerWithTimeInterval: 3
		target: self selector: @selector(reload) userInfo: nil
		repeats: YES];
	[[NSRunLoop currentRunLoop] addTimer: timer forMode: NSDefaultRunLoopMode];
}

- (void) viewWillDisappear: (BOOL)animated
{

    [self.navigationController setNavigationBarHidden:NO];
	printf("library viewWillDisappear (stopping reload timer)\n");
	[timer invalidate];
	timer = nil;
}

- (void) reload
{
	if (files)
    {
		files = nil;
	}

	NSFileManager *fileman = [NSFileManager defaultManager];
	NSString *docdir = [NSString stringWithFormat: @"%@/Documents", NSHomeDirectory()];
	NSMutableArray *outfiles = [[NSMutableArray alloc] init];
	NSDirectoryEnumerator *direnum = [fileman enumeratorAtPath:docdir];
	NSString *file;
	BOOL isdir;
	while (file = [direnum nextObject])
    {
		NSString *filepath = [docdir stringByAppendingPathComponent:file];
		NSLog(@"file %@\n", file);
		if ([fileman fileExistsAtPath:filepath isDirectory:&isdir] && !isdir) {
			[outfiles addObject:file];
		}
	}

	files = outfiles;
	[[self tableView] reloadData];
}


- (BOOL) shouldAutorotateToInterfaceOrientation: (UIInterfaceOrientation)o
{
	return YES;
}

- (NSInteger) numberOfSectionsInTableView: (UITableView*)tableView
{
	return 1;
}

- (NSInteger) tableView: (UITableView*)tableView numberOfRowsInSection: (NSInteger)section
{
	return [files count];
}

- (void) actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
	if (buttonIndex == [actionSheet destructiveButtonIndex])
	{
		char filename[PATH_MAX];
		int row = [actionSheet tag];

		dispatch_sync(queue, ^{});

		strcpy(filename, [NSHomeDirectory() UTF8String]);
		strcat(filename, "/Documents/");
		strcat(filename, [[files objectAtIndex: row] UTF8String]);

		printf("delete document '%s'\n", filename);

		unlink(filename);

		[self reload];
	}
}

- (void) onTapDelete: (UIControl*)sender
{
	int row = [sender tag];

	NSString *title = [NSString stringWithFormat: @"Delete %@?", [files objectAtIndex: row]];
	UIActionSheet *sheet = [[UIActionSheet alloc]
							initWithTitle: title
							delegate: self
							cancelButtonTitle: @"Cancel"
							destructiveButtonTitle: @"Delete"
							otherButtonTitles: nil];
	[sheet setTag: row];
	[sheet showInView: [self tableView]];
}

- (UITableViewCell*) tableView: (UITableView*)tableView cellForRowAtIndexPath: (NSIndexPath*)indexPath
{
	static NSString *cellid = @"MuCellIdent";
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier: cellid];
	if (!cell)
		cell = [[UITableViewCell alloc] initWithStyle: UITableViewCellStyleDefault reuseIdentifier: cellid];
	int row = [indexPath row];
	[[cell textLabel] setText: [files objectAtIndex: row]];
	[[cell textLabel] setFont: [UIFont systemFontOfSize: 20]];

	UIButton *deleteButton = [UIButton buttonWithType:UIButtonTypeCustom];
	[deleteButton setImage: [UIImage imageNamed: @"x_alt_blue.png"] forState: UIControlStateNormal];
	[deleteButton setFrame: CGRectMake(0, 0, 35, 35)];
	[deleteButton addTarget: self action: @selector(onTapDelete:) forControlEvents: UIControlEventTouchUpInside];
	[deleteButton setTag: row];
	[cell setAccessoryView: deleteButton];

	return cell;
}

- (void) tableView: (UITableView*)tableView didSelectRowAtIndexPath: (NSIndexPath*)indexPath
{
	int row = [indexPath row];
	[self openDocument: [files objectAtIndex: row]];
}

- (void) openDocument: (NSString*)nsfilename
{
	char filename[PATH_MAX];

	dispatch_sync(queue, ^{});

	strcpy(filename, [NSHomeDirectory() UTF8String]);
	strcat(filename, "/Documents/");
	strcat(filename, [nsfilename UTF8String]);

	printf("open document '%s'\n", filename);

	_filename = nsfilename;
	docMU = [[MuDocRef alloc] initWithFilename:filename];
	if (!docMU) {
		showAlert(@"Cannot open document", nsfilename);
		return;
	}

	if (fz_needs_password(docMU->doc))
		[self askForPassword: @"'%@' needs a password:"];
	else
		[self onPasswordOkay];
}

- (void) askForPassword: (NSString*)prompt
{
	UIAlertView *passwordAlertView = [[UIAlertView alloc]
		initWithTitle: @"Password Protected"
		message: [NSString stringWithFormat: prompt, [_filename lastPathComponent]]
		delegate: self
		cancelButtonTitle: @"Cancel"
		otherButtonTitles: @"Done", nil];
	[passwordAlertView setAlertViewStyle: UIAlertViewStyleSecureTextInput];
	[passwordAlertView show];
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	char *password = (char*) [[[alertView textFieldAtIndex: 0] text] UTF8String];
	[alertView dismissWithClickedButtonIndex: buttonIndex animated: TRUE];
	if (buttonIndex == 1) {
		if (fz_authenticate_password(docMU->doc, password))
			[self onPasswordOkay];
		else
			[self askForPassword: @"Wrong password for '%@'. Try again:"];
	} else {
		[self onPasswordCancel];
	}
}

- (void) onPasswordOkay
{
	MuDocumentController *document = [[MuDocumentController alloc] initWithFilename: _filename document: docMU];
	if (document)
    {
		[self setTitle: @"Docs"];
		[[self navigationController] pushViewController: document animated: YES];
	}
}

- (void) onPasswordCancel
{
	printf("close document (password cancel)\n");
}

@end
