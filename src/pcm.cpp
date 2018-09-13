#include "pcm.h"
#include "wave_format_exception.h"
#include "wave_header.h"

#include <cassert>
#include <iostream>
#include <istream>

using namespace std;

namespace wav2mp3 {

istream&
operator>>(istream& istr, pcm& data)
{
  //debug
  cout<<"Enter PCM::operator>>"<<endl;

  wave_header header;
  istr >> header;

  //debug
  cout<<"RIFF="<<header.riff<<endl;
  cout<<"WAVE="<<header.wave<<endl;
  cout<<"Audio Format="<<header.audio_format<<endl;
  cout<<"Bit per Sample="<<header.bits_per_sample<<endl;
  cout<<"Data Size="<<header.data_size<<endl;
  cout<<"Sample per Second="<<header.samples_per_second<<endl;
  cout<<"Number of Channel="<<header.number_of_channels<<endl;

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
   //TODO
   //for DVD
  }
  else {
    throw wave_format_exception("Unsupported Sample Rate format");
  }

  //debug
  cout<<"Check Data Size="<<header.data_size<<endl;
  cout<<"Check PCM Sample8 Size="<<sizeof(pcm::sample8)<<endl;
  cout<<"Check PCM Sample Size="<<sizeof(pcm::sample)<<endl;
  cout<<"Check PCM Sample Number="<<number_of_samples<<endl;

  vector<pcm::sample> samples;
  samples.resize(number_of_samples);

  cout<<"start format convert"<<endl;

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
   //TODO
  }
  else {
    throw wave_format_exception("Unsupported Sample Rate format");
  }

  cout<<"stop format convert"<<endl;

  swap(data.samples_, samples);
  data.samples_per_second_ =
    static_cast<pcm::samplerate>(header.samples_per_second);
  data.number_of_channels_ =
    static_cast<pcm::channels>(header.number_of_channels);

  //debug
  cout<<"Leave PCM::operator>>"<<endl;
  return istr;
}
}
