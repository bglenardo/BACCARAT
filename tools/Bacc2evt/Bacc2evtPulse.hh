#ifndef Bacc2EVTPULSE_H
#define Bacc2EVTPULSE_H 1

class Bacc2evtPulse {
  public:
    unsigned int local_event_id;
    unsigned int pulse_number;
    unsigned int channel_number;
    unsigned int pulse_header;
    unsigned long long pulse_timestamp; //Multibit?
    unsigned int trigger_flag;
    std::vector <unsigned short> data;
    double rTimeOffset;
    double gHeight;
    double gArea;
    double gSigma;
    double sphe_sigma;
    signed int pulse_start;
    unsigned int pulse_length;
    unsigned int baseline;

    Bacc2evtPulse();
    ~Bacc2evtPulse();
    double GetGain();
    double GetSpheSigma();
    unsigned int GetLocalEventID ();
    unsigned int GetPulseNumber ();
    unsigned int GetChannelNumber ();
    unsigned int GetPulseHeader (); 
    unsigned long long GetPulseTimestamp (); 
    std::vector<unsigned short> &GetData (); 
    double GetTimeOffset ();
    signed int GetPulseStart();
    unsigned int GetPulseLength (); 
    unsigned int GetBaseline (); 
    void Write();

    void SetGain(double gain); //mV ns
    void SetSpheSigma(double sphe_sig);
    void SetLocalEventID (unsigned int EID); 
    void SetPulseNumber (unsigned int pn); 
    void SetChannelNumber (unsigned int cn); 
    void SetPulseHeader (unsigned int ph); 
    void SetPulseTimestamp (unsigned long long pt); 
    void SetData (std::vector<unsigned short> d); 
    void SetTimeOffset (double rto);
    void SetPulseStart(signed int ps);
    void SetPulseLength (unsigned int pl); 
    void SetBaseline (unsigned int bl); 

    std::vector<std::vector<double> > GeneratePrePods(std::vector<double> hit_vector);
    void GeneratePulseVec (std::vector<double> &photon_time);
    std::vector<Bacc2evtPulse*> SeparatePulses();

};

#endif

