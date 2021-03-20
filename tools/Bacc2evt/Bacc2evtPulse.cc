#include <vector>
#include <iostream>
#include <math.h>

#include "Bacc2evtPulse.hh"
#include "Bacc2evtMethods.hh"

using namespace std;

// Constructors
Bacc2evtPulse::Bacc2evtPulse() {}
// Destructors
Bacc2evtPulse::~Bacc2evtPulse() {
  //this->data.clear();
}

// "Get" functions.
//double Bacc2evtPulse::GetGain(){return gHeight * (1000. / -7.26) * 16.0;}
double Bacc2evtPulse::GetGain(){return gArea;}
double Bacc2evtPulse::GetSpheSigma(){return sphe_sigma;}
unsigned int Bacc2evtPulse::GetLocalEventID () {return local_event_id;}
unsigned int Bacc2evtPulse::GetPulseNumber () {return pulse_number;}
unsigned int Bacc2evtPulse::GetChannelNumber () {return channel_number;}
unsigned int Bacc2evtPulse::GetPulseHeader () {return pulse_header;}
unsigned long long Bacc2evtPulse::GetPulseTimestamp () {
    return pulse_timestamp; }
unsigned int Bacc2evtPulse::GetPulseLength () {return pulse_length;}
unsigned int Bacc2evtPulse::GetBaseline () {return baseline;}
vector<unsigned short>& Bacc2evtPulse::GetData () {return data;}
double Bacc2evtPulse::GetTimeOffset () {return rTimeOffset;}
signed int Bacc2evtPulse::GetPulseStart() {return pulse_start;}

// "Set" functions.
//void Bacc2evtPulse::SetGain(double gain){gHeight=gain*(-7.26 / 1000.) / 16.0;}
void Bacc2evtPulse::SetGain(double gain){gArea=gain;}
void Bacc2evtPulse::SetSpheSigma(double sphe_sig){sphe_sigma = sphe_sig;}
void Bacc2evtPulse::SetLocalEventID (unsigned int EID) {local_event_id=EID;}
void Bacc2evtPulse::SetPulseNumber (unsigned int pn) {pulse_number=pn;}
void Bacc2evtPulse::SetChannelNumber (unsigned int cn) {channel_number=cn;}
void Bacc2evtPulse::SetPulseHeader (unsigned int ph) {pulse_header=ph;}
void Bacc2evtPulse::SetPulseTimestamp (unsigned long long pt) {
    pulse_timestamp=pt;}
void Bacc2evtPulse::SetPulseLength (unsigned int pl) {pulse_length=pl;}
void Bacc2evtPulse::SetBaseline (unsigned int bl) {baseline=bl;}
void Bacc2evtPulse::SetData (vector <unsigned short> d) {data=d;}
void Bacc2evtPulse::SetTimeOffset (double rto) {rTimeOffset=rto;}
void Bacc2evtPulse::SetPulseStart(signed int ps) {pulse_start=ps;}


// The good stuff

//============================================================================
//            Bacc2evtPulse::GeneratePrePods
//============================================================================
vector<vector<double> > Bacc2evtPulse::GeneratePrePods(
    vector<double> photon_times) {
  // Let's try to separate out the list of photons in to separate components
  // first. Like a pre-digitization POD mode. This prevents a GIGANTIC signal
  // signal waveform from being generated which is crucial if we are including
  // electron trains.

  // We need them to be time ordered.
  sort(photon_times.begin(), photon_times.end());

  // If there is zero or one photon, just give up and return the photon times
  // shoved in to a v<v<double>>.
  if(photon_times.size() <= 1) {
    vector<vector<double> > nearly_empty;
    nearly_empty.push_back(photon_times);
    return nearly_empty;
  }

  // Ok, now start looking for real separations. Make a vector of vectors
  // called pre_pods. This will hold subsets of the photon arrival times in
  // the photon_times that was given as an argument to this method. Any groups
  // of photon separated by longer than the specified time will be grouped in
  // to different subsets (pre-pods).
  double low_time, high_time;   // variables for the i-th an i+1-th photons
  double inter_pod_distance = 3000;    // units of ns;
  vector<vector<double> > pre_pods;
  vector<double> photon_subset;
  //cout << "i)\tsplit  \tdiff   " << endl;
  for(size_t ph=0; ph<photon_times.size()-1; ph++) {
    high_time = photon_times[ph+1];
    low_time = photon_times[ph];
    photon_subset.push_back(low_time);

    //cout << ph << "]\t" << photon_times[ph] << "\t";
    //cout << high_time - low_time << "\t";

    if(high_time - low_time > inter_pod_distance) {
      //cout << "  <--- SPLIT";
      pre_pods.push_back(photon_subset);
      photon_subset.clear();
    }
    //cout << endl;
  }

  // Ok, we've finished the loop but have a hanging photon at the end. Let's
  // check on the little guy explicitly.
  //low_time = photon_times[photon_times.size() - 2];
  //high_time = photon_times[photon_times.size() - 1];
  photon_subset.push_back(high_time);
  pre_pods.push_back(photon_subset);
  return pre_pods;

}

//============================================================================
//            Bacc2evtPulse::GeneratePulseVec
//============================================================================
void Bacc2evtPulse::GeneratePulseVec(std::vector<double> &photon_times) {
    // The photon_time marks the time of arrival of the photon on the pmt.
    unsigned int photon_times_size = photon_times.size();

    //This is where the values that determine the shape of a sphe pulse are
    //defined. Any updated PMT calibrations should be reflected in this area.
    //Eventually this might be replaced with LUG queries and/or reading xml
    //files.
    //

    // The following parameters for the sPE gaussian were obtained from pdfs
    // on the Analog Electronics Design Review page of the Twiki:
    //teacher.pas.rochester.edu:8080/wiki/bin/view/Lux/DesignReviewElectronics
    // Specifically, 090512_LUX_DAQspecs_v3.pdf and LUX_dynamic_range_v2.pdf
    // Maybe factors are calculated from Table II in the latter.

    //                          Provided at 2e6 gain
    //   _________________________________________________________________
    // /______________________/___Area___/___Height__/_Sigma_(calculated)_|
    // | PMT Output           |  8 mVns  | 0.974 mV  |       3.2 ns       |
    // | Pre Apm Output       | 40 mVns  |  4.87 mV  |       3.2 ns       |
    // | Shaping Amp Output   | 60 mVns  |  4.87 mV  |       4.9 ns       |
    // | After ADC Filtering  | 60 mVns  |  3.63 mV  |       6.6 ns       |
    // |~~~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~|~~~~~~~~~~~|~~~~~~~~~~~~~~~~~~~/ 
    //
    // Below the areas and heights have been boosted to 4e6. All values are
    // taken at the "After ADC Filtering" stage. From the above, the following
    // is true:
    //
    //  Height of a pulse at 4e6 gain is 7.26 mV = 00726 V
    //  Area of a pulse at 4e6 gain at end of the DAQ chain is 120 mVns
    //  The total amplification is 7.5x (1.5x + 5x)

    // Using the area, calculate the height of the gaussian response.
    gHeight = -0.00726 /*V*/ * ( gArea / 16.0 /*mVns*/);
    gSigma = 6.6 /*ns*/;
    double gMean;

    double digitization_freq = 100. /*MHz*/;
    double digitization_time = 1000.0 / digitization_freq; /*ns_per_sample*/

    vector<double> time_vector;
    vector<double> volt_vector;
    vector<unsigned short> adc_vector;
    vector<double> sphe_variations;

    // Since the size of a single photoelectron varies normally about a mean
    // ~16 mVns and width ~7.5 mVns for the 4e6 gain (see Franks talk from UCSB
    // colllab meeting). Let's generate the distribution of sizes.

    // Put the width of the distribution in terms of the sizes SER shown by
    // Frank so we can use the sphe_variation values as multiplicative factors.
    //double distribution_width = 7.5 /*mVns width*/ / 16. /*mVns center*/;
    /*
    distribution_width = distribution_width;
    sphe_variations.resize(photon_times_size,0);
    for(size_t photon=0; photon < photon_times_size; photon++) {
        sphe_variations[photon] = 1;//randn_trig(1.0, distribution_width);
    }
    */

    // Generate the *new* means for each photon. 
    sphe_variations.resize(photon_times_size,0);
    for(size_t photon=0; photon < photon_times_size; photon++) {
        //sphe_variations[photon] = randn_trig(1.0, distribution_width);
        sphe_variations[photon] = randn_trig(1.0, sphe_sigma / gArea );
        if(sphe_variations[photon] < 0)
          sphe_variations[photon] = 0;
        //cout << photon << ":\t" << sphe_sigma << "\t" << sphe_variations[photon] << endl;
    }

    // Maybe there should be a special case for zero length pulses?
    // Something like:
    //if (photon_times.size() == 0) {
        //this->SetData(adc_vector);
        //this->SetPulseLength(adc_vector.size());
        //this->SetBaseline(15565);

        //return;
    //}

    // The following produces data for a gaussian pulse around t=0 (ns). It
    // extends out to either side a distance of approximately 1/100th the
    // voltage resolution so that small partial overlaps may sum correctly.
    // Given arbitrary Gaussian parameters, this occurs at:
    //      sqrt(-2*gSigma**2 * log (voltageRes/ (-1.*gHeight*100.)))
    // (given gHeight is negative). This is 11.76, so round it to 12.
    // The DAQs random time offset is introduced when the gaussian is made.
    // This again is the fact that, relative to the event, the DAQs clock
    // could be sampling at some delta in time within the time resolution.



    double time = -1;    // "time" is the 
    double max_time = maxElement(photon_times);
    double min_time = minElement(photon_times);
    int loop_start = (min_time - 500)/10.;
    int loop_end   = (max_time + 1500)/10.;
    double noise=0;
    double voltage_sum;
    for(int sample= loop_start; sample < loop_end; sample++ ){
        time = sample * digitization_time + this->GetTimeOffset();
        voltage_sum = 0;
        for(unsigned int photon=0; photon < photon_times_size;photon++){
            gMean = photon_times[photon];
            if(time - gMean > 100 || time-gMean < -100)
                continue;
            voltage_sum += 
                Gauss(time, gMean, gHeight, gSigma) * sphe_variations[photon];
                //Gauss(time, gMean, gHeight, gSigma) * sphe_variations[photon];
        }
        noise = RandN() * 155e-6 /* volts RMS noise*/;
        if(Rand() - 0.5 < 0) noise *= -1;
        voltage_sum += noise;
        
        time_vector.push_back(time);
        volt_vector.push_back(voltage_sum);
        unsigned short datapoint = VtoADC(voltage_sum);
        // Add a small amount of digital noise. (source?)
        if( datapoint != 0 )    // This if statement prevents 0 underflow
          datapoint += (unsigned short) (RandN() * 1.1);
        adc_vector.push_back(datapoint);
    }
    //Uncomment for raw adc output per channel!
    //for(unsigned int j = 0; j<adc_vector.size(); j++)
    //cout << adc_vector[j]-15564 << ",";
    //cout << ";";

    this->SetData(adc_vector);
    this->SetPulseLength(adc_vector.size());
    this->SetBaseline(15564);       // Just temporary until SeparatePulses
    this->SetPulseStart(loop_start);

    return;
}

//============================================================================
//            Bacc2evtPulse::SeparatePulses
//============================================================================
std::vector<Bacc2evtPulse*> Bacc2evtPulse::SeparatePulses() {

    std::vector<unsigned short> &pulse_data = this->GetData();
    unsigned int pulse_length = this->GetPulseLength();
    int pulse_start = this->GetPulseStart();

    std::vector<Bacc2evtPulse*> pulse_vec;

    // These are the thresholds. Need to implement an xml argument instead.
    double pulse_detect_mV = -1.5; // mV.
    double pulse_end_mV    = -0.5; // mV.

    unsigned short flat_baseline= (unsigned short) mVtoADC(0);
    unsigned short pulse_detect = (unsigned short) mVtoADC(pulse_detect_mV);
    unsigned short pulse_end    = (unsigned short) mVtoADC(pulse_end_mV);

    //cout << pulse_detect_mV << "\t" << pulse_detect << endl;
    //cout << pulse_end_mV << "\t" << pulse_end << endl;

    pulse_detect = flat_baseline - pulse_detect;
    pulse_end    = flat_baseline - pulse_end;

    // These are the amount you grab before and after threshold is reached.
    unsigned short pretrigger  = 24; // in # samples.
    unsigned short posttrigger = 31; // in # samples.

    bool found_start = false;
    bool found_end = false;

    unsigned int start_sample=0;
    unsigned int end_sample=0;

    vector<unsigned short> rolling_baseline (1,flat_baseline);
    unsigned short rolling_baseline_size = 64;

    long loop_start = pulse_length; // Avoid an unsigned underflow issue due
    loop_start -= posttrigger;      // to unsigned pulse_length/posttrigger.
    for(long long i=pretrigger; i< loop_start; i++) {
        unsigned int baseline = vec_avg(rolling_baseline);
        int thresh_start = baseline - pulse_detect;
        int thresh_end = baseline - pulse_end;
        unsigned short smpl=pulse_data[i];
        // If we go above threshold while a pulse isn't found:
        if(!found_start && smpl<thresh_start){
            found_start = true;
            start_sample=i;
            //cout << "Found Start at " << i << " (below " << thresh_start << ")" << endl;
            //if( this->GetChannelNumber() == 82 )
            //cerr << "Found Start at " << i << " (below " << thresh_start << ")" << endl;
            continue;
        }

        // If we are above threshold and find the pulse go below threshold:
        if(found_start && smpl>thresh_end){
            found_end = true;
            end_sample = i;
            //if( this->GetChannelNumber() == 82 )
            //cerr << "Found End at " << i << " (above " << thresh_end << ")" << endl;
            //cout << "Found End at " << i << " (above " << thresh_end << ")" << endl;
        }

        // Continue accruing values into the rolling baseline.
        if(!found_start)
            rolling_baseline.push_back(smpl);

        // Delete the first element to reduce it to the correct size.
        if(rolling_baseline.size() > rolling_baseline_size)
            rolling_baseline.erase(rolling_baseline.begin());


        // Skip through the next "posttrigger" number of samples and look for
        // another downward going peak. If you find it, stay there and then
        // continue the loop, looking for when you go back above threshold.
        if(found_end) {
            for(unsigned int post_sample=i; 
                post_sample<i+posttrigger; post_sample++) {
                if(pulse_data[post_sample] < thresh_start){
                    i = post_sample;
                    found_end = false;
                    break;
                } // end if
            } // end for
            // Ok, so we didn't see a new part of the waveform that would have
            // exceeded threshold and made our pulse longer. However, we will 
            // check to see if there is overlap between this pulse and a
            // possible next pulse. If there is, add them together.
            //   Pulse 1          Pulse 2
            //   ,,.,./\.,..,.,.,,,/`'\.,,,.,.
            //             |--| <--overlap region.
            // should naturally become:
            //        ----Pulse 1 -----
            //   ,,.,./\.,..,.,.,,,/`'\.,,,.,.
            if(found_end) {
                unsigned int post_sample = i+posttrigger;
                unsigned int post_end = post_sample+pretrigger;
                for(post_sample=post_sample; post_sample<post_end; post_sample++) {
                    if(pulse_data[post_sample] < thresh_start) {
                        i = post_sample;
                        found_end = false;
                        break;
                    } // end if
                } // end for
            } // end if
        } // end if
              

        // Pulse finding complete. Write the sucker.
        if(found_start && found_end) {
            // An option to del the last data points to smooth baseline.
            //rolling_baseline.erase(rolling_baseline.end()-1);
            //rolling_baseline.erase(rolling_baseline.end()-1);
            //rolling_baseline.erase(rolling_baseline.end()-1);

            // Set the baseline to 1) the rolling baseline, or 2) cheat (0mV)
            baseline = vec_avg(rolling_baseline);
            //baseline = 15564;
            Bacc2evtPulse* new_pulse = new Bacc2evtPulse();
            
            unsigned int new_pulse_length;
            new_pulse_length = pretrigger + posttrigger+(end_sample-start_sample);

            std::vector<unsigned short> new_data;
            new_data.reserve(new_pulse_length);

            // Set pulse data.
            for(long long j= start_sample - pretrigger;
            //for(unsigned int j= start_sample - pretrigger;
                                j < end_sample + posttrigger; j++){
                new_data.push_back(pulse_data[j]);
            }
            // Set pulse characteristcs.
            new_pulse->SetPulseNumber(this->GetPulseNumber());
            new_pulse->SetChannelNumber(this->GetChannelNumber());
            new_pulse->SetPulseHeader(this->GetPulseHeader());
            new_pulse->SetPulseTimestamp(this->GetPulseTimestamp());
            new_pulse->SetPulseLength(new_pulse_length);
            new_pulse->SetBaseline(baseline);
            new_pulse->SetData(new_data);
            new_pulse->SetTimeOffset(this->GetTimeOffset());
            new_pulse->SetPulseStart(pulse_start + start_sample - pretrigger);


            pulse_vec.push_back(new_pulse);

            found_start = false;
            found_end   = false;
        }
    }

    // We have now gone through and separate the PODs. However, there is one
    // scenario where overlapping pods can occur due to a fluctuation in the
    // running average baseline. Let's take this in to account.
    // Loop through N-1 pulses and check if you should be combined with the
    // previous pulse due to overlap.
    for(size_t pod_ind=1; pod_ind<pulse_vec.size(); pod_ind++) {
      Bacc2evtPulse* p = pulse_vec[pod_ind];
      Bacc2evtPulse* p_minus_1 = pulse_vec[pod_ind-1];
      int end_of_first, start_of_second;
      end_of_first = p_minus_1->GetPulseStart()+p_minus_1->GetPulseLength()-1;
      start_of_second = p->GetPulseStart();
      // Look for overlap of PODs. Combine if needed.
      if(end_of_first >= start_of_second) {
        vector<unsigned short> &p_data = p->GetData();
        vector<unsigned short> &p_minus_1_data = p_minus_1->GetData();
        for(size_t dp=0; dp<p_data.size(); dp++) {
          p_minus_1_data.push_back(p_data[dp]);
        }
        // Now delete the old Bacc2evtPulse instance
        delete p;
        // and remove it from the storage vector.
        pulse_vec.erase(pulse_vec.begin() + pod_ind);
      }
    }

    return pulse_vec;
}

void Bacc2evtPulse::Write() {
  char output_file[1000];
  sprintf(output_file, "txts/%06d_%06d_%06d.txt",
    this->GetLocalEventID(), this->GetChannelNumber(), this->GetPulseNumber());
  ofstream file(output_file);
  for(long i=0; i<(long)this->GetData().size(); i++) {
    file << this->GetPulseStart() + i << "\t" << this->GetData()[i] << endl;
  }
}
