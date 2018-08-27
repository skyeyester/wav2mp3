#ifdef __APPLE__ //if OSX
//#include <filesystem>
#include <experimental/filesystem>
#else            //other OS
#include <experimental/filesystem>
#endif

namespace wav2mp3 {

using std::experimental::filesystem::path;
}
