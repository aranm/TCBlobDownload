//
//  TCBlobDownload.h
//
//  Created by Thibault Charbonnier on 15/04/13.
//  Copyright (c) 2013 Thibault Charbonnier. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void (^FirstResponseBlock)(NSURLResponse *response);
typedef void (^ProgressBlock)(float receivedLength, float totalLength);
typedef void (^ErrorBlock)(NSError *error);
typedef void (^CompleteBlock)(BOOL downloadFinished, NSString *pathToFile);

@protocol TCBlobDownloadDelegate;


#pragma mark - TCBlobDownload


@interface TCBlobDownload : NSOperation <NSURLConnectionDelegate>

@property (nonatomic, unsafe_unretained) id<TCBlobDownloadDelegate> delegate;
@property (nonatomic, copy) NSURL *downloadURL;
@property (nonatomic, copy) NSString *pathToDownloadDirectory;
@property (nonatomic, getter = pathToFile) NSString *pathToFile;
@property (nonatomic, getter = fileName) NSString *fileName;

/**
 Init. Will not start the download until you add the instanciated object to
 TCBlobDownloadManager. pathToDL cannot be nil from here.
*/
- (id)initWithUrl:(NSURL *)url
     downloadPath:(NSString *)pathToDL
      andDelegate:(id<TCBlobDownloadDelegate>)delegateOrNil;

/**
 Same but with completion blocks
*/
- (id)initWithUrl:(NSURL *)url
     downloadPath:(NSString *)pathToDL
    firstResponse:(FirstResponseBlock)firstResponseBlock
         progress:(ProgressBlock)progressBlock
            error:(ErrorBlock)errorBlock
         complete:(CompleteBlock)completeBlock;

/**
 Cancel a download and remove the file if specified.
*/
- (void)cancelDownloadAndRemoveFile:(BOOL)remove;

/**
 Make the receiver download dependent of the given download
*/
- (void)addDependentDownload:(TCBlobDownload *)blobDownload;

/**
 Create a path from given string. You should not use this method directly.
*/
+ (BOOL)createPathFromPath:(NSString *)path;

@end


#pragma mark - TCBlobDownload Delegate


@protocol TCBlobDownloadDelegate <NSObject>
@optional
/**
 Received first response
*/
- (void)download:(TCBlobDownload *)blobDownload didReceiveFirstResponse:(NSURLResponse *)response;

/**
 On each response from the NSURLConnection
*/
- (void)download:(TCBlobDownload *)blobDownload
  didReceiveData:(uint64_t)receivedLength
         onTotal:(uint64_t)totalLength;

/**
 Let you handle the error for a given download
 */
- (void)download:(TCBlobDownload *)blobDownload didStopWithError:(NSError *)error;

/**
 When a download ends
*/
- (void)download:(TCBlobDownload *)blobDownload
didFinishWithSucces:(BOOL)downloadFinished
          atPath:(NSString *)pathToFile;

@end
