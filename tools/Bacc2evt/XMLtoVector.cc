///////////////////////////////////////////////////////////////////////////////
//
//  XMLtoVector.C
//
//  Michael Woods
//  7/6/2011
//
//  Contains:
//      XMLtoVectorDbl - You want doubles?
//      XMLtoVectorInt - or would you prefer integers?
//
//  This code allows one to lightly parse XML in order to get vectors of
//  certain values. It is useful for XML like the following:
//
//  <?xml version="1.0"?>
//  <foo>
//      <bar>
//          <foobar>1.2</foobar>
//          <barfoo>13</barfoo>
//      </bar>
//      <bar>
//          <foobar>82.1</foobar>
//          <barfoo>1.6e2</barfoo>
//      </bar>
//      <bar>
//          <foobar>32</foobar>
//          <barfoo>1.e39</barfoo>
//      </bar>
//      <bar>
//          <foobar>39</foobar>
//          <barfoo>0.001</barfoo>
//      </bar>
//  </foo>
//
//  XMLtoVectorDbl(that_xml, "barfoo") will return a vector containing:
//      vector<double> {13, 1.6e2, 1e39, 0.001}
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include "XMLtoVector.hh"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

vector<double> XMLtoVectorDbl(string xml, string search_term) {
    
    string start_tag = "<" + search_term + ">"; // <PMT_number>
    string end_tag  = "</" + search_term + ">"; // </PMT_number>

    //bool found_end = false;

    size_t start_tag_pos = 0;       // <PMT_number> Where is the "<"?
    size_t end_tag_pos   = 0;       // </PMT_number> Where is the "<"?
    size_t last_pos      = 0;

    size_t size = xml.size();

    size_t number_start;
    size_t number_end;
    string a_number;

    vector<double> result_vector;

    while(true) {
        start_tag_pos = xml.find(start_tag, last_pos+1);
        end_tag_pos = xml.find(end_tag, start_tag_pos);
        //cout << start_tag_pos << "\t" << end_tag_pos << endl;

        // Make sure to exit if we are done.
        if(start_tag_pos > size) break;

        number_start = start_tag_pos + start_tag.size();
        number_end   = end_tag_pos;
        last_pos = start_tag_pos;

        a_number = xml.substr(number_start, number_end-number_start);

        //cout << "*" << a_number << endl;
        // Strip whitespace (spaces, tabs, newlines) out of the result.
        for(size_t i=a_number.size()-1; i!=a_number.npos; i--) {
            char c = a_number[i];
            if(c == ' ' || c == '\n' || c == '\t'){
                a_number.replace(i,1,"");
                //a_number.erase(i,1);      // This fails in ROOT 5.28!?
            }
        }
        result_vector.push_back(atof(a_number.c_str()));
        

    }

    return result_vector;
}

///////////////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

vector<int> XMLtoVectorInt(string xml, string search_term) {
    
    string start_tag = "<" + search_term + ">"; // <PMT_number>
    string end_tag  = "</" + search_term + ">"; // </PMT_number>

    //bool found_end = false;

    size_t start_tag_pos = 0;       // <PMT_number> Where is the "<"?
    size_t end_tag_pos   = 0;       // </PMT_number> Where is the "<"?
    size_t last_pos      = 0;

    size_t size = xml.size();

    size_t number_start;
    size_t number_end;
    string a_number;

    vector<int> result_vector;

    while(true) {
        start_tag_pos = xml.find(start_tag, last_pos+1);
        end_tag_pos = xml.find(end_tag, start_tag_pos);

        // Make sure to exit if we are done.
        if(start_tag_pos > size) break;

        number_start = start_tag_pos + start_tag.size();
        number_end   = end_tag_pos;
        last_pos = start_tag_pos;

        a_number = xml.substr(number_start, number_end-number_start);

        // Strip whitespace (spaces, tabs, newlines) out of the result.
        for(size_t i=a_number.size()-1; i!=a_number.npos; i--) {
            char c = a_number[i];
            if(c == ' ' || c == '\n' || c == '\t'){
                a_number.replace(i,1,"");
                //a_number.erase(i,1);      // This fails in ROOT 5.28!?
            }
        }
        result_vector.push_back(atof(a_number.c_str()));
        
    }

    return result_vector;
}

