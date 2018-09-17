#include "wave_files.h"
#include "wave_format_exception.h"
#include "wave_header.h"

#include <algorithm>
#include <cctype>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>

//uncomment if want to print log
//#define DEBUG

using namespace std;
using experimental::filesystem::directory_iterator;

namespace wav2mp3 {
namespace {
vector<path>
directory_entries(path const& directory)
{
  vector<path> entries;

  for (auto entry : directory_iterator(directory))
    entries.push_back(entry);

  return entries;
}
bool
has_wave_file_extension(path const& filename)
{
  auto extension = path::string_type{ filename.extension() };
  transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

  return extension == path{ ".wav" };
}
bool
is_wave_file(path const& filename)
{
  wave_header header;

  try {
    if (ifstream{ filename, ifstream::binary } >> header)
      return true;
  } catch (wave_format_exception const&) {
  }

  return false;
}
}

vector<path>
wave_files(path const& directory)
{
  vector<path> files;

  for (auto entry : directory_entries(directory)) {
#ifdef DEBUG
	  cout << "File Entry= " << entry.c_str() << endl;
	  cout << "has_wave_file_extension= " << has_wave_file_extension(entry) << endl;
	  cout << "is_wave_file Entry= " << is_wave_file(entry) << endl;
#endif
	  if (has_wave_file_extension(entry) && is_wave_file(entry))
		  files.push_back(entry);
  }
  return files;
}
}
