#ifndef PUBLIC_ASSERTEX_H_
#define PUBLIC_ASSERTEX_H_

void onAssert(char const * const  file, int line);

#ifdef NASSERT

#define DEFINE_THIS_FILE
#define DEFINE_THIS_MODULE(name_)
#define ASSERT(test_)    ((void)0)
#define ALLEGE(test_)    ((void)(test_))
#define ERROR()          ((void)0)

#else
#define DEFINE_THIS_FILE \
static char const l_this_file[] = __FILE__;
#define DEFINE_THIS_MODULE(name_) \
static char const l_this_file[] = #name_;
#define ASSERT(test_) \
if (test_) { \
} \
else (onAssert(l_this_file, __LINE__))
#define ALLEGE(test_)    ASSERT(test_)
#define ERROR() \
(onAssert(l_this_file, __LINE__))
#endif

#define REQUIRE(test_)   ASSERT(test_)
#define ENSURE(test_)    ASSERT(test_



#endif /* PUBLIC_ASSERTEX_H_ */
