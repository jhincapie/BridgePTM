//
//  PMBlocks.h
//

typedef void (^VoidCompletion)(void);

typedef void (^BoolCompletion)(BOOL result);

typedef void (^DataErrorCompletion)(NSData *data, NSError *error);

typedef void (^ListErrorCompletion)(NSArray *results, NSError* error);

