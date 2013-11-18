//
//  PMDocumentsViewController.m
//  BridgeMobile
//

#import "PMDocumentsViewController.h"
#import "PMDocumentCell.h"

@interface PMDocumentsViewController ()

@property (nonatomic, strong) NSArray *documents;

@end

@implementation PMDocumentsViewController


- (void)viewDidLoad{
    
    [super viewDidLoad];
    
    self.title = NSLocalizedString(@"", nil);
    
    [self.context documentList:^(NSArray *results, NSError *error) {
       
        self.documents = results;
        
        [self.tableView reloadData];
        
    }];
    
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
    
    NSString *document = [self.documents objectAtIndex:indexPath.row];
    
    cell.descriptionLabel.text = document;
    
    return cell;
    
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    return 87;
    
}

@end
