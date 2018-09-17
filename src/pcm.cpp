#include "pcm.h"
#include "wave_format_exception.h"
#include "wave_header.h"

#include <cassert>
#include <iostream>
#include <istream>

//uncomment if want to print log
//#define DEBUG

using namespace std;

namespace wav2mp3 {

istream&
operator>>(istream& istr, pcm& data)
{

#ifdef DEBUG
  cout<<"Enter PCM::operator>>"<<endl;
#endif

  wave_header header;
  istr >> header;

#ifdef DEBUG
  cout<<"RIFF="<<header.riff<<endl;
  cout<<"WAVE="<<header.wave<<endl;
  cout<<"Audio Format="<<header.audio_format<<endl;
  cout<<"Bit per Sample="<<header.bits_per_sample<<endl;
  cout<<"Data Size="<<header.data_size<<endl;
  cout<<"Sample per Second="<<header.samples_per_second<<endl;
  cout<<"Number of Channel="<<header.number_of_channels<<endl;
#endif

  if (header.audio_format != wave_header::PCM){
    throw wave_format_exception("Unsupported Audio format");
  }

  if (header.number_of_channels != 1 && header.number_of_channels != 2){
    throw wave_format_exception("Unsupported Channel format");
  }

  auto const bytes_per_sample = header.bits_per_sample / 8;
  auto const number_of_samples = header.data_size / bytes_per_sample;

  if (header.bits_per_sample == 8) {
    assert(header.data_size == sizeof(pcm::sample8) * number_of_samples);
  } else if (header.bits_per_sample == 12) {
    //TODO
  } else if (header.bits_per_sample == 16) {
    //for CD
    assert(header.data_size == sizeof(pcm::sample) * number_of_samples);
  } else if (header.bits_per_sample == 24) {
	//for DVD
	assert(header.data_size == bytes_per_sample * number_of_samples);
  }
  else {
    throw wave_format_exception("Unsupported Sample Rate format");
  }

#ifdef DEBUG
  cout<<"Check Data Size="<<header.data_size<<endl;
  cout<<"Check PCM Sample8 Size="<<sizeof(pcm::sample8)<<endl;
  cout<<"Check PCM Sample Size="<<sizeof(pcm::sample)<<endl;
  cout<<"Check PCM Sample Number="<<number_of_samples<<endl;
#endif

  vector<pcm::sample> samples;
  samples.resize(number_of_samples);

#ifdef DEBUG
  cout<<"start format convert"<<endl;
#endif

  if (header.bits_per_sample == 8) {

      vector<pcm::sample8> sample8s;
      sample8s.resize(number_of_samples);
      if (!istr.read(reinterpret_cast<char*>(sample8s.data()), header.data_size))
          throw wave_format_exception("Unexpected end of file");
      //do format convert
      short temp = 0;
      for(int i=0; i<sample8s.size(); i++) {
        temp = (short)(sample8s.at(i) - 0x80) <<8;
        samples.at(i) = temp;
      }
  } else if (header.bits_per_sample == 12) {
    //TODO
  } else if (header.bits_per_sample == 16) {
      if (!istr.read(reinterpret_cast<char*>(samples.data()), header.data_size))
          throw wave_format_exception("Unexpected end of file");
  } else if (header.bits_per_sample == 24) {
	//read byte one by one
	vector<pcm::sample8> sample8s;
	sample8s.resize(header.data_size);
	if (!istr.read(reinterpret_cast<char*>(sample8s.data()), header.data_size))
		throw wave_format_exception("Unexpected end of file");
	//do format convert
	short temp = 0;
	int index = 0;
	for (int i = 0; i < sample8s.size(); i += 3) {
		temp = ((short)(sample8s.at(i+2) - 0x80) << 8)+ ((short)(sample8s.at(i+1) - 0x80));
		//skipped the first bytes in every sample because little-endian
		index = i / 3;
		samples.at(index) = temp;
	}
  }
  else {
    throw wave_format_exception("Unsupported Sample Rate format");
  }

#ifdef DEBUG
  cout<<"stop format convert"<<endl;
#endif

  swap(data.samples_, samples);
  data.samples_per_second_ =
    static_cast<pcm::samplerate>(header.samples_per_second);
  data.number_of_channels_ =
    static_cast<pcm::channels>(header.number_of_channels);

#ifdef DEBUG
  cout<<"Leave PCM::operator>>"<<endl;
#endif
  return istr;
}
}
