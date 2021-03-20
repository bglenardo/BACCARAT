// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME BaccMCTruthEvent_dict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "BaccMCTruthEvent.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *photonMCTruth_Dictionary();
   static void photonMCTruth_TClassManip(TClass*);
   static void *new_photonMCTruth(void *p = 0);
   static void *newArray_photonMCTruth(Long_t size, void *p);
   static void delete_photonMCTruth(void *p);
   static void deleteArray_photonMCTruth(void *p);
   static void destruct_photonMCTruth(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::photonMCTruth*)
   {
      ::photonMCTruth *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::photonMCTruth));
      static ::ROOT::TGenericClassInfo 
         instance("photonMCTruth", "BaccMCTruthEvent.hh", 46,
                  typeid(::photonMCTruth), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &photonMCTruth_Dictionary, isa_proxy, 4,
                  sizeof(::photonMCTruth) );
      instance.SetNew(&new_photonMCTruth);
      instance.SetNewArray(&newArray_photonMCTruth);
      instance.SetDelete(&delete_photonMCTruth);
      instance.SetDeleteArray(&deleteArray_photonMCTruth);
      instance.SetDestructor(&destruct_photonMCTruth);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::photonMCTruth*)
   {
      return GenerateInitInstanceLocal((::photonMCTruth*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::photonMCTruth*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *photonMCTruth_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::photonMCTruth*)0x0)->GetClass();
      photonMCTruth_TClassManip(theClass);
   return theClass;
   }

   static void photonMCTruth_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *vertexMCTruth_Dictionary();
   static void vertexMCTruth_TClassManip(TClass*);
   static void *new_vertexMCTruth(void *p = 0);
   static void *newArray_vertexMCTruth(Long_t size, void *p);
   static void delete_vertexMCTruth(void *p);
   static void deleteArray_vertexMCTruth(void *p);
   static void destruct_vertexMCTruth(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::vertexMCTruth*)
   {
      ::vertexMCTruth *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::vertexMCTruth));
      static ::ROOT::TGenericClassInfo 
         instance("vertexMCTruth", "BaccMCTruthEvent.hh", 56,
                  typeid(::vertexMCTruth), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vertexMCTruth_Dictionary, isa_proxy, 4,
                  sizeof(::vertexMCTruth) );
      instance.SetNew(&new_vertexMCTruth);
      instance.SetNewArray(&newArray_vertexMCTruth);
      instance.SetDelete(&delete_vertexMCTruth);
      instance.SetDeleteArray(&deleteArray_vertexMCTruth);
      instance.SetDestructor(&destruct_vertexMCTruth);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::vertexMCTruth*)
   {
      return GenerateInitInstanceLocal((::vertexMCTruth*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::vertexMCTruth*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vertexMCTruth_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::vertexMCTruth*)0x0)->GetClass();
      vertexMCTruth_TClassManip(theClass);
   return theClass;
   }

   static void vertexMCTruth_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *pmtMCTruth_Dictionary();
   static void pmtMCTruth_TClassManip(TClass*);
   static void *new_pmtMCTruth(void *p = 0);
   static void *newArray_pmtMCTruth(Long_t size, void *p);
   static void delete_pmtMCTruth(void *p);
   static void deleteArray_pmtMCTruth(void *p);
   static void destruct_pmtMCTruth(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::pmtMCTruth*)
   {
      ::pmtMCTruth *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::pmtMCTruth));
      static ::ROOT::TGenericClassInfo 
         instance("pmtMCTruth", "BaccMCTruthEvent.hh", 73,
                  typeid(::pmtMCTruth), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &pmtMCTruth_Dictionary, isa_proxy, 4,
                  sizeof(::pmtMCTruth) );
      instance.SetNew(&new_pmtMCTruth);
      instance.SetNewArray(&newArray_pmtMCTruth);
      instance.SetDelete(&delete_pmtMCTruth);
      instance.SetDeleteArray(&deleteArray_pmtMCTruth);
      instance.SetDestructor(&destruct_pmtMCTruth);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::pmtMCTruth*)
   {
      return GenerateInitInstanceLocal((::pmtMCTruth*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::pmtMCTruth*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *pmtMCTruth_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::pmtMCTruth*)0x0)->GetClass();
      pmtMCTruth_TClassManip(theClass);
   return theClass;
   }

   static void pmtMCTruth_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_BaccMCTruthEvent(void *p = 0);
   static void *newArray_BaccMCTruthEvent(Long_t size, void *p);
   static void delete_BaccMCTruthEvent(void *p);
   static void deleteArray_BaccMCTruthEvent(void *p);
   static void destruct_BaccMCTruthEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::BaccMCTruthEvent*)
   {
      ::BaccMCTruthEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::BaccMCTruthEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("BaccMCTruthEvent", ::BaccMCTruthEvent::Class_Version(), "BaccMCTruthEvent.hh", 77,
                  typeid(::BaccMCTruthEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::BaccMCTruthEvent::Dictionary, isa_proxy, 4,
                  sizeof(::BaccMCTruthEvent) );
      instance.SetNew(&new_BaccMCTruthEvent);
      instance.SetNewArray(&newArray_BaccMCTruthEvent);
      instance.SetDelete(&delete_BaccMCTruthEvent);
      instance.SetDeleteArray(&deleteArray_BaccMCTruthEvent);
      instance.SetDestructor(&destruct_BaccMCTruthEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::BaccMCTruthEvent*)
   {
      return GenerateInitInstanceLocal((::BaccMCTruthEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::BaccMCTruthEvent*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr BaccMCTruthEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *BaccMCTruthEvent::Class_Name()
{
   return "BaccMCTruthEvent";
}

//______________________________________________________________________________
const char *BaccMCTruthEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BaccMCTruthEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int BaccMCTruthEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BaccMCTruthEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *BaccMCTruthEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BaccMCTruthEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *BaccMCTruthEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BaccMCTruthEvent*)0x0)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_photonMCTruth(void *p) {
      return  p ? new(p) ::photonMCTruth : new ::photonMCTruth;
   }
   static void *newArray_photonMCTruth(Long_t nElements, void *p) {
      return p ? new(p) ::photonMCTruth[nElements] : new ::photonMCTruth[nElements];
   }
   // Wrapper around operator delete
   static void delete_photonMCTruth(void *p) {
      delete ((::photonMCTruth*)p);
   }
   static void deleteArray_photonMCTruth(void *p) {
      delete [] ((::photonMCTruth*)p);
   }
   static void destruct_photonMCTruth(void *p) {
      typedef ::photonMCTruth current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::photonMCTruth

namespace ROOT {
   // Wrappers around operator new
   static void *new_vertexMCTruth(void *p) {
      return  p ? new(p) ::vertexMCTruth : new ::vertexMCTruth;
   }
   static void *newArray_vertexMCTruth(Long_t nElements, void *p) {
      return p ? new(p) ::vertexMCTruth[nElements] : new ::vertexMCTruth[nElements];
   }
   // Wrapper around operator delete
   static void delete_vertexMCTruth(void *p) {
      delete ((::vertexMCTruth*)p);
   }
   static void deleteArray_vertexMCTruth(void *p) {
      delete [] ((::vertexMCTruth*)p);
   }
   static void destruct_vertexMCTruth(void *p) {
      typedef ::vertexMCTruth current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::vertexMCTruth

namespace ROOT {
   // Wrappers around operator new
   static void *new_pmtMCTruth(void *p) {
      return  p ? new(p) ::pmtMCTruth : new ::pmtMCTruth;
   }
   static void *newArray_pmtMCTruth(Long_t nElements, void *p) {
      return p ? new(p) ::pmtMCTruth[nElements] : new ::pmtMCTruth[nElements];
   }
   // Wrapper around operator delete
   static void delete_pmtMCTruth(void *p) {
      delete ((::pmtMCTruth*)p);
   }
   static void deleteArray_pmtMCTruth(void *p) {
      delete [] ((::pmtMCTruth*)p);
   }
   static void destruct_pmtMCTruth(void *p) {
      typedef ::pmtMCTruth current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::pmtMCTruth

//______________________________________________________________________________
void BaccMCTruthEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class BaccMCTruthEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(BaccMCTruthEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(BaccMCTruthEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_BaccMCTruthEvent(void *p) {
      return  p ? new(p) ::BaccMCTruthEvent : new ::BaccMCTruthEvent;
   }
   static void *newArray_BaccMCTruthEvent(Long_t nElements, void *p) {
      return p ? new(p) ::BaccMCTruthEvent[nElements] : new ::BaccMCTruthEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_BaccMCTruthEvent(void *p) {
      delete ((::BaccMCTruthEvent*)p);
   }
   static void deleteArray_BaccMCTruthEvent(void *p) {
      delete [] ((::BaccMCTruthEvent*)p);
   }
   static void destruct_BaccMCTruthEvent(void *p) {
      typedef ::BaccMCTruthEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::BaccMCTruthEvent

namespace ROOT {
   static TClass *vectorlEvertexMCTruthgR_Dictionary();
   static void vectorlEvertexMCTruthgR_TClassManip(TClass*);
   static void *new_vectorlEvertexMCTruthgR(void *p = 0);
   static void *newArray_vectorlEvertexMCTruthgR(Long_t size, void *p);
   static void delete_vectorlEvertexMCTruthgR(void *p);
   static void deleteArray_vectorlEvertexMCTruthgR(void *p);
   static void destruct_vectorlEvertexMCTruthgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vertexMCTruth>*)
   {
      vector<vertexMCTruth> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vertexMCTruth>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vertexMCTruth>", -2, "vector", 447,
                  typeid(vector<vertexMCTruth>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvertexMCTruthgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<vertexMCTruth>) );
      instance.SetNew(&new_vectorlEvertexMCTruthgR);
      instance.SetNewArray(&newArray_vectorlEvertexMCTruthgR);
      instance.SetDelete(&delete_vectorlEvertexMCTruthgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvertexMCTruthgR);
      instance.SetDestructor(&destruct_vectorlEvertexMCTruthgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vertexMCTruth> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vertexMCTruth>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvertexMCTruthgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vertexMCTruth>*)0x0)->GetClass();
      vectorlEvertexMCTruthgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvertexMCTruthgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvertexMCTruthgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vertexMCTruth> : new vector<vertexMCTruth>;
   }
   static void *newArray_vectorlEvertexMCTruthgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vertexMCTruth>[nElements] : new vector<vertexMCTruth>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvertexMCTruthgR(void *p) {
      delete ((vector<vertexMCTruth>*)p);
   }
   static void deleteArray_vectorlEvertexMCTruthgR(void *p) {
      delete [] ((vector<vertexMCTruth>*)p);
   }
   static void destruct_vectorlEvertexMCTruthgR(void *p) {
      typedef vector<vertexMCTruth> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vertexMCTruth>

namespace ROOT {
   static TClass *vectorlEpmtMCTruthgR_Dictionary();
   static void vectorlEpmtMCTruthgR_TClassManip(TClass*);
   static void *new_vectorlEpmtMCTruthgR(void *p = 0);
   static void *newArray_vectorlEpmtMCTruthgR(Long_t size, void *p);
   static void delete_vectorlEpmtMCTruthgR(void *p);
   static void deleteArray_vectorlEpmtMCTruthgR(void *p);
   static void destruct_vectorlEpmtMCTruthgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pmtMCTruth>*)
   {
      vector<pmtMCTruth> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pmtMCTruth>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pmtMCTruth>", -2, "vector", 447,
                  typeid(vector<pmtMCTruth>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpmtMCTruthgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<pmtMCTruth>) );
      instance.SetNew(&new_vectorlEpmtMCTruthgR);
      instance.SetNewArray(&newArray_vectorlEpmtMCTruthgR);
      instance.SetDelete(&delete_vectorlEpmtMCTruthgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpmtMCTruthgR);
      instance.SetDestructor(&destruct_vectorlEpmtMCTruthgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pmtMCTruth> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<pmtMCTruth>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpmtMCTruthgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pmtMCTruth>*)0x0)->GetClass();
      vectorlEpmtMCTruthgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpmtMCTruthgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpmtMCTruthgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pmtMCTruth> : new vector<pmtMCTruth>;
   }
   static void *newArray_vectorlEpmtMCTruthgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pmtMCTruth>[nElements] : new vector<pmtMCTruth>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpmtMCTruthgR(void *p) {
      delete ((vector<pmtMCTruth>*)p);
   }
   static void deleteArray_vectorlEpmtMCTruthgR(void *p) {
      delete [] ((vector<pmtMCTruth>*)p);
   }
   static void destruct_vectorlEpmtMCTruthgR(void *p) {
      typedef vector<pmtMCTruth> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pmtMCTruth>

namespace ROOT {
   static TClass *vectorlEphotonMCTruthgR_Dictionary();
   static void vectorlEphotonMCTruthgR_TClassManip(TClass*);
   static void *new_vectorlEphotonMCTruthgR(void *p = 0);
   static void *newArray_vectorlEphotonMCTruthgR(Long_t size, void *p);
   static void delete_vectorlEphotonMCTruthgR(void *p);
   static void deleteArray_vectorlEphotonMCTruthgR(void *p);
   static void destruct_vectorlEphotonMCTruthgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<photonMCTruth>*)
   {
      vector<photonMCTruth> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<photonMCTruth>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<photonMCTruth>", -2, "vector", 447,
                  typeid(vector<photonMCTruth>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEphotonMCTruthgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<photonMCTruth>) );
      instance.SetNew(&new_vectorlEphotonMCTruthgR);
      instance.SetNewArray(&newArray_vectorlEphotonMCTruthgR);
      instance.SetDelete(&delete_vectorlEphotonMCTruthgR);
      instance.SetDeleteArray(&deleteArray_vectorlEphotonMCTruthgR);
      instance.SetDestructor(&destruct_vectorlEphotonMCTruthgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<photonMCTruth> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<photonMCTruth>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEphotonMCTruthgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<photonMCTruth>*)0x0)->GetClass();
      vectorlEphotonMCTruthgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEphotonMCTruthgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEphotonMCTruthgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<photonMCTruth> : new vector<photonMCTruth>;
   }
   static void *newArray_vectorlEphotonMCTruthgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<photonMCTruth>[nElements] : new vector<photonMCTruth>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEphotonMCTruthgR(void *p) {
      delete ((vector<photonMCTruth>*)p);
   }
   static void deleteArray_vectorlEphotonMCTruthgR(void *p) {
      delete [] ((vector<photonMCTruth>*)p);
   }
   static void destruct_vectorlEphotonMCTruthgR(void *p) {
      typedef vector<photonMCTruth> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<photonMCTruth>

namespace {
  void TriggerDictionaryInitialization_BaccMCTruthEvent_dict_Impl() {
    static const char* headers[] = {
"BaccMCTruthEvent.hh",
0
    };
    static const char* includePaths[] = {
"/Users/blenardo/Research/Software/root-6.12.06-build/include",
"/Users/blenardo/Research/TMS/Simulations/BACCARAT/tools/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "BaccMCTruthEvent_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
struct __attribute__((annotate("$clingAutoload$BaccMCTruthEvent.hh")))  photonMCTruth;
namespace std{inline namespace __1{template <class _Tp> class __attribute__((annotate("$clingAutoload$iosfwd")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}}
struct __attribute__((annotate("$clingAutoload$BaccMCTruthEvent.hh")))  vertexMCTruth;
struct __attribute__((annotate("$clingAutoload$BaccMCTruthEvent.hh")))  pmtMCTruth;
class __attribute__((annotate("$clingAutoload$BaccMCTruthEvent.hh")))  BaccMCTruthEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "BaccMCTruthEvent_dict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "BaccMCTruthEvent.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"BaccMCTruthEvent", payloadCode, "@",
"photonMCTruth", payloadCode, "@",
"pmtMCTruth", payloadCode, "@",
"vertexMCTruth", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("BaccMCTruthEvent_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_BaccMCTruthEvent_dict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_BaccMCTruthEvent_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_BaccMCTruthEvent_dict() {
  TriggerDictionaryInitialization_BaccMCTruthEvent_dict_Impl();
}
