//
//  PMAppContext.m
//  BridgeMobile
//

#import "PMAppContext.h"

@implementation PMAppContext

- (void)documentList:(ListErrorCompletion)completion{

    NSFileManager *fileman = [NSFileManager defaultManager];
	
    NSString *docdir = [NSString stringWithFormat: @"%@/Documents", NSHomeDirectory()];
	
    NSMutableArray *outfiles = [[NSMutableArray alloc] init];
	
    NSDirectoryEnumerator *direnum = [fileman enumeratorAtPath:docdir];
	
    NSString *file;
	
    BOOL isdir;
	
    while (file = [direnum nextObject]){
        
		NSString *filepath = [docdir stringByAppendingPathComponent:file];
		
		if ([fileman fileExistsAtPath:filepath isDirectory:&isdir] && !isdir) {
            
			[outfiles addObject:file];
            
		}
        
	}
    
    if(completion != nil){
    
        completion(outfiles, nil);
    
    }

}

@end
