@import CoreGraphics.CGGeometry;

#define CGRectGetCenter(inrect) ({ \
    CGRect const rect = inrect; \
    CGPointMake(rect.origin.x + rect.size.width / 2.f, rect.origin.y + rect.size.height / 2.f); \
})

#define CGRectMakeSquare(center, length) ({ \
    CGPoint const cc = center; \
    CGFloat const ll = length; \
    CGRectMake(cc.x - ll/2.f, cc.y - ll/2.f, ll, ll); \
})


//////////////////////////////////////////////////////////////////////// NSArray
#define NSArrayMap(array, block) ({ \
    id o; id mapped[array.count]; \
    NSUInteger jj = 0; \
    for (id mappable in array) \
        if ((o = block(mappable))) mapped[jj++] = o; \
    [NSArray arrayWithObjects:mapped count:jj]; \
})

#define NSArraySelect(array, block) NSArrayMap(array, ^(id obj){ \
    return block(obj) ? obj : nil; \
})


////////////////////////////////////////////////////////////////// NSFileManager
@import Foundation.NSPathUtilities;

#define NSDocumentsDirectory(...) ({ \
    id dir = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject]; \
    [dir stringByAppendingPathComponent:[@[__VA_ARGS__] componentsJoinedByString:@"/"]]; \
})

#define NSCacheDirectory(...) ({ \
    id dir = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) lastObject]; \
    [dir stringByAppendingPathComponent:[@[__VA_ARGS__] componentsJoinedByString:@"/"]]; \
})


//////////////////////////////////////////////////////////// NSHTTPCookieStorage
@import Foundation.NSHTTPCookieStorage;

#define NSHTTPCookieStorageEmpty() { \
    NSHTTPCookieStorage *storage = [NSHTTPCookieStorage sharedHTTPCookieStorage]; \
    for (id cookie in storage.cookies) \
        [storage deleteCookie:cookie]; \
}


/////////////////////////////////////////////////////////////////////// NSObject
#define NSObjectDumpSelectors(o) { \
    unsigned n = 0; \
    Method *methods = class_copyMethodList([o class], &n); \
    for (int i = 0; i < n; ++i) \
        NSLog(@"%@", NSStringFromSelector(method_getName(methods[i]))); \
    free(methods); \
}


/////////////////////////////////////////////////////////////////////// NSString
@import Foundation.NSString;

#define NSStringGrep(str, rx) \
    ([str rangeOfString:rx options:NSRegularExpressionSearch].location != NSNotFound)

#define NSStringGetRange(str) \
    NSMakeRange(0, [str length])

#define NSStringFromCLLocationCoordinate2D(coordinate) \
    [NSString stringWithFormat:@"(%f,%f)", coordinate.latitude, coordinate.longitude]

#define NSStringChomp(s) ({ \
    NSUInteger const N = [s length]; \
    N ? [s stringByReplacingCharactersInRange:NSMakeRange(N-1, 1) withString:@""] : s; \
})


///////////////////////////////////////////////////////////////// NSUserDefaults
@import Foundation.NSUserDefaults;

static inline void NSUserDefaultsSync(void(^block)(NSUserDefaults const*const defaults)) {
    NSUserDefaults const*const defaults = [NSUserDefaults standardUserDefaults];
    block(defaults);
    [defaults synchronize];
}


//////////////////////////////////////////////////////////////////// SecKeychain
#define SecKeychainReset() { \
    void (^rm)(CFTypeRef) = ^(CFTypeRef secClass) { \
        id dict = @{(__bridge id)kSecClass: (__bridge id)secClass}; \
        OSStatus result = SecItemDelete((__bridge CFDictionaryRef) dict); \
        NSAssert(result == noErr || result == errSecItemNotFound, @"Error deleting keychain data (%d)", (int)result); \
    }; \
    rm(kSecClassGenericPassword); \
    rm(kSecClassInternetPassword); \
    rm(kSecClassCertificate); \
    rm(kSecClassKey); \
    rm(kSecClassIdentity); \
}


////////////////////////////////////////////////////////////////////// SKProduct
#define SKProductPriceString(product) ({ \
    NSNumberFormatter *nf = [NSNumberFormatter new]; \
    [nf setFormatterBehavior:NSNumberFormatterBehavior10_4]; \
    [nf setNumberStyle:NSNumberFormatterCurrencyStyle]; \
    [nf setLocale:product.priceLocale]; \
    [nf stringFromNumber:product.price]; \
})


//////////////////////////////////////////////////////////////////////// UIColor
#define UIColorFromRGB(rgbValue) \
    [UIColor colorWithRed:((float)((rgbValue & 0xFF0000) >> 16))/255.0 \
                    green:((float)((rgbValue & 0xFF00) >> 8))/255.0 \
                     blue:((float)(rgbValue & 0xFF))/255.0 \
                    alpha:1.0]


/////////////////////////////////////////////////////////////////////// UIDevice
@import Foundation.NSObjCRuntime;

#define UIDeviceSystemMajorVersion() \
    (NSFoundationVersionNumber >= 1134.10 \
        ? 8 \
        : NSFoundationVersionNumber >= 1047.00 \
            ? 7 \
                : NSFoundationVersionNumber >= 993.00 \
                ? 6 \
                : 5)


///////////////////////////////////////////////////////////////////////// UIFont
#define UIFontDumpAll() { \
    for (id obj in UIFont.familyNames) \
        NSLog(@"%@: %@", obj, [UIFont fontNamesForFamilyName:obj]); \
}


//////////////////////////////////////////////////////////////////// UIResponder
#define UIResponderResignFirst() \
    [[UIApplication sharedApplication] sendAction:@selector(resignFirstResponder) to:nil from:nil forEvent:nil]


/////////////////////////////////////////////////////////////////////// UIScreen
#define UINavigationBarHeight 44.f
#define UIScreenHeight UIScreen.mainScreen.bounds.size.height
#define UIScreenWidth UIScreen.mainScreen.bounds.size.width
#define UIScreenCenter CGRectGetCenter(UIScreen.mainScreen.bounds)
#define UIStatusBarHeight 20.f
#define UIKeyboardHeight 216.f
#define UIToolbarHeightLandscape 32.f
#define UITextFieldHeight 32.f
#define UIPageControlHeight 37.f

#define UIScreen320x480() \
    ({ UIScreenHeight == 480; })

#define UIScreen320x568() \
    ({ UIScreenHeight == 568; })
        
#define UIScreen375x667() \
    ({ UIScreenHeight == 667; })

#define UIScreen414x736() \
    ({ UIScreenHeight == 736; })


///////////////////////////////////////////////////////////////////////// UIView
#define UIViewAllSubviews(rootview) ({ \
    NSMutableArray *stack = [NSMutableArray arrayWithObjects:rootview, nil]; \
    NSMutableArray *views = [NSMutableArray array]; \
    while (stack.count) { \
        id subviews = [stack[0] subviews]; \
        [views addObjectsFromArray:subviews]; \
        [stack addObjectsFromArray:subviews]; \
        [stack removeObjectAtIndex:0]; \
    } \
    views; \
})

#define UIViewParentController(view) ({ \
    UIResponder *responder = view; \
    while ([responder isKindOfClass:[UIView class]]) \
        responder = [responder nextResponder]; \
    (UIViewController *)responder; \
})


/////////////////////////////////////////////////////////////////////////// else
#define __assert_main_thread {assert(NSThread.isMainThread);}
#define __assert_not_main_thread {assert(!NSThread.isMainThread);}
#define __mxcl_retain(...) void *retainedThing = (__bridge_retained void *)__VA_ARGS__; retainedThing = retainedThing
#define __mxcl_release(...) void *retainedThing = (__bridge void *) __VA_ARGS__; id unretainedThing = (__bridge_transfer id)retainedThing; unretainedThing = nil
#define __mxcl_bound(min, value, max) ({ typeof(value) cp = (value); MAX(min, MIN(max, cp)); })

#define M_GOLDEN_RATIO 1.61803398875
