//
//  PMDocumentsViewController.m
//  BridgeMobile
//
//  Created by Andrés Castaño on 13/11/2013.
//  Copyright (c) 2013 Artifex Software, Inc. All rights reserved.
//

#import "PMDocumentsViewController.h"
#import "PMDocumentCell.h"

@interface PMDocumentsViewController ()

@property (nonatomic, retain) NSArray *documents;

@end

@implementation PMDocumentsViewController


- (void)viewDidLoad{
    
    [super viewDidLoad];
    
    self.title = NSLocalizedString(@"", nil);
    
}

- (void)didReceiveMemoryWarning{
    
    [super didReceiveMemoryWarning];
    
}

- (void)viewDidUnload {
    
    [self setTableView:nil];
    
    [super viewDidUnload];
    
}

#pragma mark - UITableView Data Source

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    
    return self.documents.count;
    
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    PMDocumentCell *cell = (PMDocumentCell *)[tableView dequeueReusableCellWithIdentifier:@"Cell"];
    
    if (cell == nil) {
        
        NSArray	*nib = [[NSBundle mainBundle] loadNibNamed:@"PMDocumentCell" owner:self options:nil];
        
        for (id nibElement in nib) {
            
            if ([nibElement isKindOfClass:[PMDocumentCell class]]) {
                
                cell = nibElement;
                
                break;
                
            }
            
        }
        
    }
    
    return cell;
    
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    return 87;
    
}

@end
