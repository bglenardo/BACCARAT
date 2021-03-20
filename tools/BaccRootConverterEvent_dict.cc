// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME BaccRootConverterEvent_dict

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
#include "BaccRootConverterEvent.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *volumeInfo_Dictionary();
   static void volumeInfo_TClassManip(TClass*);
   static void *new_volumeInfo(void *p = 0);
   static void *newArray_volumeInfo(Long_t size, void *p);
   static void delete_volumeInfo(void *p);
   static void deleteArray_volumeInfo(void *p);
   static void destruct_volumeInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::volumeInfo*)
   {
      ::volumeInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::volumeInfo));
      static ::ROOT::TGenericClassInfo 
         instance("volumeInfo", "BaccRootConverterEvent.hh", 45,
                  typeid(::volumeInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &volumeInfo_Dictionary, isa_proxy, 4,
                  sizeof(::volumeInfo) );
      instance.SetNew(&new_volumeInfo);
      instance.SetNewArray(&newArray_volumeInfo);
      instance.SetDelete(&delete_volumeInfo);
      instance.SetDeleteArray(&deleteArray_volumeInfo);
      instance.SetDestructor(&destruct_volumeInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::volumeInfo*)
   {
      return GenerateInitInstanceLocal((::volumeInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::volumeInfo*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *volumeInfo_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::volumeInfo*)0x0)->GetClass();
      volumeInfo_TClassManip(theClass);
   return theClass;
   }

   static void volumeInfo_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *primaryParticleInfo_Dictionary();
   static void primaryParticleInfo_TClassManip(TClass*);
   static void *new_primaryParticleInfo(void *p = 0);
   static void *newArray_primaryParticleInfo(Long_t size, void *p);
   static void delete_primaryParticleInfo(void *p);
   static void deleteArray_primaryParticleInfo(void *p);
   static void destruct_primaryParticleInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::primaryParticleInfo*)
   {
      ::primaryParticleInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::primaryParticleInfo));
      static ::ROOT::TGenericClassInfo 
         instance("primaryParticleInfo", "BaccRootConverterEvent.hh", 53,
                  typeid(::primaryParticleInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &primaryParticleInfo_Dictionary, isa_proxy, 4,
                  sizeof(::primaryParticleInfo) );
      instance.SetNew(&new_primaryParticleInfo);
      instance.SetNewArray(&newArray_primaryParticleInfo);
      instance.SetDelete(&delete_primaryParticleInfo);
      instance.SetDeleteArray(&deleteArray_primaryParticleInfo);
      instance.SetDestructor(&destruct_primaryParticleInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::primaryParticleInfo*)
   {
      return GenerateInitInstanceLocal((::primaryParticleInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::primaryParticleInfo*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *primaryParticleInfo_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::primaryParticleInfo*)0x0)->GetClass();
      primaryParticleInfo_TClassManip(theClass);
   return theClass;
   }

   static void primaryParticleInfo_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *stepInfo_Dictionary();
   static void stepInfo_TClassManip(TClass*);
   static void *new_stepInfo(void *p = 0);
   static void *newArray_stepInfo(Long_t size, void *p);
   static void delete_stepInfo(void *p);
   static void deleteArray_stepInfo(void *p);
   static void destruct_stepInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::stepInfo*)
   {
      ::stepInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::stepInfo));
      static ::ROOT::TGenericClassInfo 
         instance("stepInfo", "BaccRootConverterEvent.hh", 63,
                  typeid(::stepInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &stepInfo_Dictionary, isa_proxy, 4,
                  sizeof(::stepInfo) );
      instance.SetNew(&new_stepInfo);
      instance.SetNewArray(&newArray_stepInfo);
      instance.SetDelete(&delete_stepInfo);
      instance.SetDeleteArray(&deleteArray_stepInfo);
      instance.SetDestructor(&destruct_stepInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::stepInfo*)
   {
      return GenerateInitInstanceLocal((::stepInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::stepInfo*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *stepInfo_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::stepInfo*)0x0)->GetClass();
      stepInfo_TClassManip(theClass);
   return theClass;
   }

   static void stepInfo_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *trackInfo_Dictionary();
   static void trackInfo_TClassManip(TClass*);
   static void *new_trackInfo(void *p = 0);
   static void *newArray_trackInfo(Long_t size, void *p);
   static void delete_trackInfo(void *p);
   static void deleteArray_trackInfo(void *p);
   static void destruct_trackInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::trackInfo*)
   {
      ::trackInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::trackInfo));
      static ::ROOT::TGenericClassInfo 
         instance("trackInfo", "BaccRootConverterEvent.hh", 75,
                  typeid(::trackInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &trackInfo_Dictionary, isa_proxy, 4,
                  sizeof(::trackInfo) );
      instance.SetNew(&new_trackInfo);
      instance.SetNewArray(&newArray_trackInfo);
      instance.SetDelete(&delete_trackInfo);
      instance.SetDeleteArray(&deleteArray_trackInfo);
      instance.SetDestructor(&destruct_trackInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::trackInfo*)
   {
      return GenerateInitInstanceLocal((::trackInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::trackInfo*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *trackInfo_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::trackInfo*)0x0)->GetClass();
      trackInfo_TClassManip(theClass);
   return theClass;
   }

   static void trackInfo_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_BaccRootConverterEvent(void *p = 0);
   static void *newArray_BaccRootConverterEvent(Long_t size, void *p);
   static void delete_BaccRootConverterEvent(void *p);
   static void deleteArray_BaccRootConverterEvent(void *p);
   static void destruct_BaccRootConverterEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::BaccRootConverterEvent*)
   {
      ::BaccRootConverterEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::BaccRootConverterEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("BaccRootConverterEvent", ::BaccRootConverterEvent::Class_Version(), "BaccRootConverterEvent.hh", 86,
                  typeid(::BaccRootConverterEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::BaccRootConverterEvent::Dictionary, isa_proxy, 4,
                  sizeof(::BaccRootConverterEvent) );
      instance.SetNew(&new_BaccRootConverterEvent);
      instance.SetNewArray(&newArray_BaccRootConverterEvent);
      instance.SetDelete(&delete_BaccRootConverterEvent);
      instance.SetDeleteArray(&deleteArray_BaccRootConverterEvent);
      instance.SetDestructor(&destruct_BaccRootConverterEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::BaccRootConverterEvent*)
   {
      return GenerateInitInstanceLocal((::BaccRootConverterEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::BaccRootConverterEvent*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr BaccRootConverterEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *BaccRootConverterEvent::Class_Name()
{
   return "BaccRootConverterEvent";
}

//______________________________________________________________________________
const char *BaccRootConverterEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BaccRootConverterEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int BaccRootConverterEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BaccRootConverterEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *BaccRootConverterEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BaccRootConverterEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *BaccRootConverterEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BaccRootConverterEvent*)0x0)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_volumeInfo(void *p) {
      return  p ? new(p) ::volumeInfo : new ::volumeInfo;
   }
   static void *newArray_volumeInfo(Long_t nElements, void *p) {
      return p ? new(p) ::volumeInfo[nElements] : new ::volumeInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_volumeInfo(void *p) {
      delete ((::volumeInfo*)p);
   }
   static void deleteArray_volumeInfo(void *p) {
      delete [] ((::volumeInfo*)p);
   }
   static void destruct_volumeInfo(void *p) {
      typedef ::volumeInfo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::volumeInfo

namespace ROOT {
   // Wrappers around operator new
   static void *new_primaryParticleInfo(void *p) {
      return  p ? new(p) ::primaryParticleInfo : new ::primaryParticleInfo;
   }
   static void *newArray_primaryParticleInfo(Long_t nElements, void *p) {
      return p ? new(p) ::primaryParticleInfo[nElements] : new ::primaryParticleInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_primaryParticleInfo(void *p) {
      delete ((::primaryParticleInfo*)p);
   }
   static void deleteArray_primaryParticleInfo(void *p) {
      delete [] ((::primaryParticleInfo*)p);
   }
   static void destruct_primaryParticleInfo(void *p) {
      typedef ::primaryParticleInfo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::primaryParticleInfo

namespace ROOT {
   // Wrappers around operator new
   static void *new_stepInfo(void *p) {
      return  p ? new(p) ::stepInfo : new ::stepInfo;
   }
   static void *newArray_stepInfo(Long_t nElements, void *p) {
      return p ? new(p) ::stepInfo[nElements] : new ::stepInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_stepInfo(void *p) {
      delete ((::stepInfo*)p);
   }
   static void deleteArray_stepInfo(void *p) {
      delete [] ((::stepInfo*)p);
   }
   static void destruct_stepInfo(void *p) {
      typedef ::stepInfo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::stepInfo

namespace ROOT {
   // Wrappers around operator new
   static void *new_trackInfo(void *p) {
      return  p ? new(p) ::trackInfo : new ::trackInfo;
   }
   static void *newArray_trackInfo(Long_t nElements, void *p) {
      return p ? new(p) ::trackInfo[nElements] : new ::trackInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_trackInfo(void *p) {
      delete ((::trackInfo*)p);
   }
   static void deleteArray_trackInfo(void *p) {
      delete [] ((::trackInfo*)p);
   }
   static void destruct_trackInfo(void *p) {
      typedef ::trackInfo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::trackInfo

//______________________________________________________________________________
void BaccRootConverterEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class BaccRootConverterEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(BaccRootConverterEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(BaccRootConverterEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_BaccRootConverterEvent(void *p) {
      return  p ? new(p) ::BaccRootConverterEvent : new ::BaccRootConverterEvent;
   }
   static void *newArray_BaccRootConverterEvent(Long_t nElements, void *p) {
      return p ? new(p) ::BaccRootConverterEvent[nElements] : new ::BaccRootConverterEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_BaccRootConverterEvent(void *p) {
      delete ((::BaccRootConverterEvent*)p);
   }
   static void deleteArray_BaccRootConverterEvent(void *p) {
      delete [] ((::BaccRootConverterEvent*)p);
   }
   static void destruct_BaccRootConverterEvent(void *p) {
      typedef ::BaccRootConverterEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::BaccRootConverterEvent

namespace ROOT {
   static TClass *vectorlEvolumeInfogR_Dictionary();
   static void vectorlEvolumeInfogR_TClassManip(TClass*);
   static void *new_vectorlEvolumeInfogR(void *p = 0);
   static void *newArray_vectorlEvolumeInfogR(Long_t size, void *p);
   static void delete_vectorlEvolumeInfogR(void *p);
   static void deleteArray_vectorlEvolumeInfogR(void *p);
   static void destruct_vectorlEvolumeInfogR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<volumeInfo>*)
   {
      vector<volumeInfo> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<volumeInfo>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<volumeInfo>", -2, "vector", 214,
                  typeid(vector<volumeInfo>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvolumeInfogR_Dictionary, isa_proxy, 4,
                  sizeof(vector<volumeInfo>) );
      instance.SetNew(&new_vectorlEvolumeInfogR);
      instance.SetNewArray(&newArray_vectorlEvolumeInfogR);
      instance.SetDelete(&delete_vectorlEvolumeInfogR);
      instance.SetDeleteArray(&deleteArray_vectorlEvolumeInfogR);
      instance.SetDestructor(&destruct_vectorlEvolumeInfogR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<volumeInfo> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<volumeInfo>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvolumeInfogR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<volumeInfo>*)0x0)->GetClass();
      vectorlEvolumeInfogR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvolumeInfogR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvolumeInfogR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<volumeInfo> : new vector<volumeInfo>;
   }
   static void *newArray_vectorlEvolumeInfogR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<volumeInfo>[nElements] : new vector<volumeInfo>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvolumeInfogR(void *p) {
      delete ((vector<volumeInfo>*)p);
   }
   static void deleteArray_vectorlEvolumeInfogR(void *p) {
      delete [] ((vector<volumeInfo>*)p);
   }
   static void destruct_vectorlEvolumeInfogR(void *p) {
      typedef vector<volumeInfo> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<volumeInfo>

namespace ROOT {
   static TClass *vectorlEtrackInfogR_Dictionary();
   static void vectorlEtrackInfogR_TClassManip(TClass*);
   static void *new_vectorlEtrackInfogR(void *p = 0);
   static void *newArray_vectorlEtrackInfogR(Long_t size, void *p);
   static void delete_vectorlEtrackInfogR(void *p);
   static void deleteArray_vectorlEtrackInfogR(void *p);
   static void destruct_vectorlEtrackInfogR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<trackInfo>*)
   {
      vector<trackInfo> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<trackInfo>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<trackInfo>", -2, "vector", 214,
                  typeid(vector<trackInfo>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEtrackInfogR_Dictionary, isa_proxy, 4,
                  sizeof(vector<trackInfo>) );
      instance.SetNew(&new_vectorlEtrackInfogR);
      instance.SetNewArray(&newArray_vectorlEtrackInfogR);
      instance.SetDelete(&delete_vectorlEtrackInfogR);
      instance.SetDeleteArray(&deleteArray_vectorlEtrackInfogR);
      instance.SetDestructor(&destruct_vectorlEtrackInfogR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<trackInfo> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<trackInfo>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEtrackInfogR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<trackInfo>*)0x0)->GetClass();
      vectorlEtrackInfogR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEtrackInfogR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEtrackInfogR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<trackInfo> : new vector<trackInfo>;
   }
   static void *newArray_vectorlEtrackInfogR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<trackInfo>[nElements] : new vector<trackInfo>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEtrackInfogR(void *p) {
      delete ((vector<trackInfo>*)p);
   }
   static void deleteArray_vectorlEtrackInfogR(void *p) {
      delete [] ((vector<trackInfo>*)p);
   }
   static void destruct_vectorlEtrackInfogR(void *p) {
      typedef vector<trackInfo> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<trackInfo>

namespace ROOT {
   static TClass *vectorlEstepInfogR_Dictionary();
   static void vectorlEstepInfogR_TClassManip(TClass*);
   static void *new_vectorlEstepInfogR(void *p = 0);
   static void *newArray_vectorlEstepInfogR(Long_t size, void *p);
   static void delete_vectorlEstepInfogR(void *p);
   static void deleteArray_vectorlEstepInfogR(void *p);
   static void destruct_vectorlEstepInfogR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<stepInfo>*)
   {
      vector<stepInfo> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<stepInfo>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<stepInfo>", -2, "vector", 214,
                  typeid(vector<stepInfo>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEstepInfogR_Dictionary, isa_proxy, 4,
                  sizeof(vector<stepInfo>) );
      instance.SetNew(&new_vectorlEstepInfogR);
      instance.SetNewArray(&newArray_vectorlEstepInfogR);
      instance.SetDelete(&delete_vectorlEstepInfogR);
      instance.SetDeleteArray(&deleteArray_vectorlEstepInfogR);
      instance.SetDestructor(&destruct_vectorlEstepInfogR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<stepInfo> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<stepInfo>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstepInfogR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<stepInfo>*)0x0)->GetClass();
      vectorlEstepInfogR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstepInfogR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstepInfogR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<stepInfo> : new vector<stepInfo>;
   }
   static void *newArray_vectorlEstepInfogR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<stepInfo>[nElements] : new vector<stepInfo>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstepInfogR(void *p) {
      delete ((vector<stepInfo>*)p);
   }
   static void deleteArray_vectorlEstepInfogR(void *p) {
      delete [] ((vector<stepInfo>*)p);
   }
   static void destruct_vectorlEstepInfogR(void *p) {
      typedef vector<stepInfo> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<stepInfo>

namespace ROOT {
   static TClass *vectorlEprimaryParticleInfogR_Dictionary();
   static void vectorlEprimaryParticleInfogR_TClassManip(TClass*);
   static void *new_vectorlEprimaryParticleInfogR(void *p = 0);
   static void *newArray_vectorlEprimaryParticleInfogR(Long_t size, void *p);
   static void delete_vectorlEprimaryParticleInfogR(void *p);
   static void deleteArray_vectorlEprimaryParticleInfogR(void *p);
   static void destruct_vectorlEprimaryParticleInfogR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<primaryParticleInfo>*)
   {
      vector<primaryParticleInfo> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<primaryParticleInfo>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<primaryParticleInfo>", -2, "vector", 214,
                  typeid(vector<primaryParticleInfo>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEprimaryParticleInfogR_Dictionary, isa_proxy, 4,
                  sizeof(vector<primaryParticleInfo>) );
      instance.SetNew(&new_vectorlEprimaryParticleInfogR);
      instance.SetNewArray(&newArray_vectorlEprimaryParticleInfogR);
      instance.SetDelete(&delete_vectorlEprimaryParticleInfogR);
      instance.SetDeleteArray(&deleteArray_vectorlEprimaryParticleInfogR);
      instance.SetDestructor(&destruct_vectorlEprimaryParticleInfogR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<primaryParticleInfo> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<primaryParticleInfo>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEprimaryParticleInfogR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<primaryParticleInfo>*)0x0)->GetClass();
      vectorlEprimaryParticleInfogR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEprimaryParticleInfogR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEprimaryParticleInfogR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<primaryParticleInfo> : new vector<primaryParticleInfo>;
   }
   static void *newArray_vectorlEprimaryParticleInfogR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<primaryParticleInfo>[nElements] : new vector<primaryParticleInfo>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEprimaryParticleInfogR(void *p) {
      delete ((vector<primaryParticleInfo>*)p);
   }
   static void deleteArray_vectorlEprimaryParticleInfogR(void *p) {
      delete [] ((vector<primaryParticleInfo>*)p);
   }
   static void destruct_vectorlEprimaryParticleInfogR(void *p) {
      typedef vector<primaryParticleInfo> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<primaryParticleInfo>

namespace {
  void TriggerDictionaryInitialization_BaccRootConverterEvent_dict_Impl() {
    static const char* headers[] = {
"BaccRootConverterEvent.hh",
0
    };
    static const char* includePaths[] = {
"/usr/gdata/cern/root_v6.12.02/toss_3_x86_64-install/include",
"/g/g20/lenardo1/Simulations/BACCARAT/tools/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "BaccRootConverterEvent_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
struct __attribute__((annotate("$clingAutoload$BaccRootConverterEvent.hh")))  stepInfo;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
struct __attribute__((annotate("$clingAutoload$BaccRootConverterEvent.hh")))  volumeInfo;
struct __attribute__((annotate("$clingAutoload$BaccRootConverterEvent.hh")))  primaryParticleInfo;
struct __attribute__((annotate("$clingAutoload$BaccRootConverterEvent.hh")))  trackInfo;
class __attribute__((annotate("$clingAutoload$BaccRootConverterEvent.hh")))  BaccRootConverterEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "BaccRootConverterEvent_dict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "BaccRootConverterEvent.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"BaccRootConverterEvent", payloadCode, "@",
"primaryParticleInfo", payloadCode, "@",
"stepInfo", payloadCode, "@",
"trackInfo", payloadCode, "@",
"volumeInfo", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("BaccRootConverterEvent_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_BaccRootConverterEvent_dict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_BaccRootConverterEvent_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_BaccRootConverterEvent_dict() {
  TriggerDictionaryInitialization_BaccRootConverterEvent_dict_Impl();
}
