// -*- C++ -*-
//
// Package:    demo/OOD0Skim
// Class:      OOD0Skim
//
/**\class OOD0Skim OOD0Skim.cc demo/OOD0Skim/plugins/OOD0Skim.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Zhenyu Chen
// Updated Author:  Jiateng Peng
//         Created:  23 Dec 2025
// New contents: the efficiency of trigger particles are constrained to 1
//

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>

#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TNtuple.h>
#include <TFile.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TRandom.h>
#include <TLorentzVector.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidateFwd.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"


#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <Math/Functions.h>
#include <Math/SVector.h>
#include <Math/SMatrix.h>


#define PI 3.1416

using namespace std;

using reco::TrackCollection;
using reco::VertexCompositeCandidateCollection;

class OOD0Skim : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit OOD0Skim(const edm::ParameterSet&);
  ~OOD0Skim() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void fillRECO(const edm::Event&, const edm::EventSetup&);
  void endJob() override;

  // ----------member data ---------------------------
    edm::ParameterSet theConfig;
    edm::EDGetTokenT<edm::View<pat::PackedCandidate>> token_tracks;
    edm::EDGetTokenT<reco::VertexCollection> token_vertices;
    edm::EDGetTokenT<reco::VertexCompositeCandidateCollection> recoVertexCompositeCandidateCollection_Token_;

    //static const int nCentBins = 1;
    //float cBin[nCentBins+1]={0,10};

    static const int MinCent = 0;
    static const int MaxCent = 19;

    static const int nCentTableBins = 200;
    //Official O table
    float binBoundaries[nCentTableBins+1]={0,0.717572,1.43514,2.15272,2.87029,3.58786,4.30543,5.023,5.74057,6.45815,7.17572,7.89329,8.61086,9.32843,10.046,10.5615,10.9358,11.3222,11.7114,12.1054,12.5071,12.9114,13.3242,13.7529,14.1895,14.6323,15.0788,15.5384,16.0017,16.4718,16.954,17.4529,17.965,18.4818,19.0081,19.5384,20.0798,20.6295,21.1904,21.7624,22.3333,22.9144,23.5082,24.108,24.7103,25.3327,25.9643,26.6021,27.2404,27.8889,28.551,29.2201,29.8838,30.5609,31.2582,31.959,32.6755,33.3996,34.127,34.8736,35.624,36.3835,37.1513,37.9462,38.7306,39.5312,40.3522,41.1784,41.9996,42.8382,43.7007,44.5735,45.4614,46.3438,47.2291,48.1364,49.0614,49.9993,50.9485,51.9135,52.8921,53.8808,54.8964,55.893,56.9136,57.9568,59.0177,60.0888,61.1833,62.2824,63.399,64.5337,65.6807,66.8337,68.0214,69.219,70.4243,71.651,72.9014,74.165,75.4546,76.7537,78.0703,79.4045,80.7569,82.1322,83.5298,84.9421,86.3744,87.8423,89.3304,90.8548,92.361,93.9159,95.4992,97.0799,98.6655,100.303,101.96,103.647,105.32,107.036,108.77,110.55,112.332,114.116,115.936,117.785,119.708,121.683,123.605,125.589,127.588,129.583,131.654,133.746,135.887,138.016,140.191,142.401,144.623,146.867,149.14,151.514,153.909,156.277,158.74,161.209,163.662,166.177,168.728,171.298,173.901,176.575,179.302,182.038,184.793,187.576,190.47,193.355,196.279,199.296,202.355,205.406,208.567,211.746,215.007,218.296,221.624,224.981,228.464,231.972,235.5,239.147,242.785,246.538,250.308,254.2,258.246,262.353,266.531,270.819,275.228,279.794,284.51,289.356,294.368,299.561,305.114,310.888,316.984,323.451,330.516,337.981,346.265,355.527,366.193,379.001,395.659,421.298,10000.0};

    static const int nMassBins = 14;
    float massCuts[nMassBins+1]={1.700,1.740,1.780,1.800,1.820,1.840,1.850,1.860,1.865,1.870,1.880,1.900,1.920,1.960,2.000 };

    static const int nPtBins = 8;
    float ptCuts[nPtBins+1]={1.5,2.4,3.0,3.5,4.2,5.0,6.0,7.0,8.0 };

    TH1D* hNtrk;

    TH1D* hpt;
    TH1D* heta;
    TH1D* hphi;
    TH1D* hrapidity;
    TH1D* hmass;
    
    TH3F* effhisto;
    TH3F* fakehisto;
    TH3F* sechisto;
    TH3F* mulhisto;

    struct MassBinHistograms {
        TH1D* hMult_bins;
        TH2D* hSignal;
        TH2D* hBackground;
    };

    struct Histograms {
        TH1D* hmass;
        std::vector<MassBinHistograms> PtMassBins_SB;
    };

    std::vector<Histograms> PtBins_all;

    struct Particle_ass {
        float eta;
        float phi;
        float pt; 
    };

    struct Particle_trg {
        float eta;
        float phi;
        float pt; 
        float eta_dau1;
        float phi_dau1;
        float eta_dau2;
        float phi_dau2;
    };

    vector< vector< vector< vector<Particle_trg> > > > *pVectVectVect_trg;
    vector<Particle_ass> *pVect_ass;
    vector< vector<Particle_ass> >  *pVectVect_ass;
    
    vector<float> *zvtxVect;
    vector<int> *nNtrkoffline;
    vector<int> *ncent;
    
    int nMult_Ntrkoffline;
    float bestvx;
    float bestvy;
    float bestvz;
    
    
    float y;
    float mass;
    float dzos1;
    float dzos2;
    float dxyos1;
    float dxyos2;
    float VtxProb;
    int nhit1;
    int nhit2;
    float dlos;
    float dl;
    float dlerror;
    float agl;
    bool trkquality1;
    bool trkquality2;
    float pt1;
    float pt2;
    float ptErr1;
    float ptErr2;
    float p1;
    float p2;
    float eta1;
    float eta2;
    float phi1;
    float phi2;
    float charge1;
    float charge2;
    float vtxChi2;
    float ndf;
    float agl_abs;
    float agl2D;
    float agl2D_abs;
    float dlos2D;
    float trackdca;
    float d0dca;
    float trkChi1;
    float trkChi2;
    bool isSwap;
    bool isPrompt;
    int idmom_reco;
    bool matchGEN;    


    float tolerance = 0.0002;

};

OOD0Skim::OOD0Skim(const edm::ParameterSet& theParameters) :
    theConfig(theParameters)
    {
        //now do what ever other initialization is needed
        using std::string;
        token_tracks = consumes<edm::View<pat::PackedCandidate>>(theParameters.getParameter<edm::InputTag>("TrackLabel"));
        token_vertices = consumes<reco::VertexCollection>(theParameters.getParameter<edm::InputTag>("VertexLabel"));

        recoVertexCompositeCandidateCollection_Token_ = consumes<reco::VertexCompositeCandidateCollection>(theConfig.getUntrackedParameter<edm::InputTag>("VertexCompositeCollection"));

    
    }

OOD0Skim::~OOD0Skim() {

    delete pVectVectVect_trg;
    delete pVectVect_ass;
    delete zvtxVect;
    delete nNtrkoffline;
    delete ncent;

}
void OOD0Skim::analyze(const edm::Event &iEvent, const edm::EventSetup &iSetup)
{
    using std::vector;
    using std::cout;
    using std::endl;
    using namespace reco;
    using namespace edm;

    // Get the vertex
    Handle<reco::VertexCollection> theVertexHandle;
    iEvent.getByToken(token_vertices, theVertexHandle);
    if(theVertexHandle->size() == 0) return;
    

    // select on requirement of valid vertex
    bestvz=-999.9; 
    bestvx=-999.9; 
    bestvy=-999.9;
    double bestvzError=-999.9, bestvxError=-999.9, bestvyError=-999.9;

    const reco::VertexCollection vtxCollection = *(theVertexHandle.product());
    reco::VertexCollection::const_iterator vtxPrimary = vtxCollection.begin();

    if(vtxPrimary->isFake()) return;

    bestvx = vtxPrimary->x();
    bestvy = vtxPrimary->y();
    bestvz = vtxPrimary->z();
    bestvzError = vtxPrimary->zError(); 
    bestvxError = vtxPrimary->xError(); 
    bestvyError = vtxPrimary->yError();

    if (bestvz > 15.0 || bestvz < -15.0) return;

    math::XYZPoint bestvtx(bestvx, bestvy, bestvz);

    // Get the tracks
    auto theTrackHandle = iEvent.getHandle(token_tracks);
    if (!theTrackHandle->size()) return;

    // do the track collection
    auto input_cands = *theTrackHandle;
    edm::View<pat::PackedCandidate>::const_iterator indx;

    int nMult_Ntrkoffline = 0;
    float toppt=-999;

    for (indx = input_cands.begin(); indx != input_cands.end(); indx++)
    {
        // select tracks
        if (!indx->hasTrackDetails())continue;
        if (abs(indx->charge()) != 1)continue;
        if (!(indx->pseudoTrack().quality(reco::TrackBase::qualityByName("highPurity"))))continue;

        const Track &tmpTrack = indx->pseudoTrack();

        // DCA cut
        float dzvtx = tmpTrack.dz(bestvtx);
        float dxyvtx = tmpTrack.dxy(bestvtx);
        float dzerror = sqrt(tmpTrack.dzError() * tmpTrack.dzError() + bestvzError * bestvzError);
        float dxyerror = sqrt(tmpTrack.d0Error() * tmpTrack.d0Error() + bestvxError * bestvyError);

        if (fabs(tmpTrack.ptError()) / tmpTrack.pt() > 0.10)continue;
        if (fabs(dzvtx / dzerror) > 3)continue;
        if (fabs(dxyvtx / dxyerror) > 3)continue;
        if (fabs(dzvtx) > 9999.0) continue;

        if (tmpTrack.eta() > -2.4 && tmpTrack.eta() < 2.4 && tmpTrack.pt() > 0.4)
            nMult_Ntrkoffline++;
        if (tmpTrack.eta() < 2.4 && tmpTrack.eta() > -2.4 &&  tmpTrack.pt() > toppt)
            toppt = tmpTrack.pt();

    }
    
    if(nMult_Ntrkoffline < 0 || nMult_Ntrkoffline > 2000) return;
    if(toppt < 0 || toppt > 9999.0) return;
    hNtrk->Fill(nMult_Ntrkoffline);

    float etHFtowerSumPlus = 0;
    float etHFtowerSumMinus = 0;
    float etHFtowerSum = 0;

    //use sum of et to sperate centrality bins
    for(indx=input_cands.begin(); indx != input_cands.end(); indx++) {

            if(indx->pdgId() == 1 || indx->pdgId() == 2)  {
                //HF+
                if(indx->eta() > 3.0 && indx->eta() < 5.2) etHFtowerSumPlus += indx->et();
                //HF-
                if(indx->eta() > -5.2 && indx->eta() < -3.0) etHFtowerSumMinus += indx->et();
            }
    } 

    etHFtowerSum = etHFtowerSumPlus + etHFtowerSumMinus;

    int cent = -1;
    for (int ic=0; ic < nCentTableBins; ic++){
        if(etHFtowerSum>binBoundaries[ic] && etHFtowerSum<binBoundaries[ic+1]){
        cent = nCentTableBins -1 - ic;
        }
    }

    if(cent<MinCent || cent>MaxCent) return;

    //get associated particles
    pVect_ass = new vector<Particle_ass>;

    for (indx = input_cands.begin(); indx != input_cands.end(); indx++){
            if (!indx->hasTrackDetails())continue;
            if (abs(indx->charge()) != 1)continue;
            if (!(indx->pseudoTrack().quality(reco::TrackBase::qualityByName("highPurity"))))continue;

            const Track &tmpTrack = indx->pseudoTrack();

            float dzvtx = tmpTrack.dz(bestvtx);
            float dxyvtx = tmpTrack.dxy(bestvtx);
            float dzerror = sqrt(tmpTrack.dzError() * tmpTrack.dzError() + bestvzError * bestvzError);
            float dxyerror = sqrt(tmpTrack.d0Error() * tmpTrack.d0Error() + bestvxError * bestvyError);

            if (fabs(tmpTrack.ptError()) / tmpTrack.pt() > 0.10)continue;
            if (fabs(dzvtx / dzerror) > 3)continue;
            if (fabs(dxyvtx / dxyerror) > 3)continue;
            if (fabs(dzvtx) > 9999.0) continue;

            float eta = tmpTrack.eta();
            float phi = tmpTrack.phi();
            float pt  = tmpTrack.pt();

            Particle_ass part;
            part.pt = pt;
            part.eta = eta;
            part.phi = phi;

            if(eta<=2.4 && eta>=-2.4 && pt<=3.0 && pt>=0.3) pVect_ass->push_back(part);
    }

    std::vector<std::vector<std::vector<Particle_trg>>> particlesPerBin(nPtBins, std::vector<std::vector<Particle_trg>>(nMassBins));

    // Get the V0s
    edm::Handle<reco::VertexCompositeCandidateCollection> D0candidates;
    iEvent.getByToken(recoVertexCompositeCandidateCollection_Token_,D0candidates);
    if(!D0candidates.isValid()) return;
    const reco::VertexCompositeCandidateCollection * D0candidates_ = D0candidates.product();

    for(unsigned it=0; it<D0candidates_->size(); ++it){
        
        const reco::VertexCompositeCandidate & trk = (*D0candidates_)[it];

        double secvz=-999.9, secvx=-999.9, secvy=-999.9;
        //double secvzError=-999.9, secvxError=-999.9, secvyError=-999.9;
        //pt,mass
        float eta = trk.eta();
        //if(eta>2.4 || eta<-2.4) continue;
        float pt = trk.pt();
        float phi = trk.phi();
        y = trk.rapidity();
        if(y > 1.0 || y < -1.0) continue;

        double px = trk.px();
        double py = trk.py();
        double pz = trk.pz();
        mass = trk.mass();
        
        const reco::Candidate * d1 = trk.daughter(0);
        const reco::Candidate * d2 = trk.daughter(1);
        
        //Gen match
        matchGEN = false;
        isSwap = false;
        isPrompt = true;
        idmom_reco = -77;
        
        double pxd1 = d1->px();
        double pyd1 = d1->py();
        double pzd1 = d1->pz();
        //double pd1 = d1->p();
        double pxd2 = d2->px();
        double pyd2 = d2->py();
        double pzd2 = d2->pz();
        //double pd2 = d2->p();
        
        TVector3 dauvec1(pxd1,pyd1,pzd1);
        TVector3 dauvec2(pxd2,pyd2,pzd2);
        
        auto dau1 = d1->get<reco::TrackRef>();
        auto dau2 = d2->get<reco::TrackRef>();
        
        
        //trk quality
        
        trkquality1 = dau1->quality(reco::TrackBase::highPurity);
        trkquality2 = dau2->quality(reco::TrackBase::highPurity);
        
        if(!dau1->quality(reco::TrackBase::highPurity)) continue;
        if(!dau2->quality(reco::TrackBase::highPurity)) continue;


        //track pt
        pt1 = d1->pt();
        pt2 = d2->pt();
        
        //track momentum
        p1 = d1->p();
        p2 = d2->p();
        
        //track eta
        eta1 = d1->eta();
        eta2 = d2->eta();

        //track eta
        phi1 = d1->phi();
        phi2 = d2->phi();
        
        //track charge
        charge1 = d1->charge();
        charge2 = d2->charge();

        //track Chi2
        trkChi1 = dau1->normalizedChi2();
        trkChi2 = dau2->normalizedChi2();
        
        //track pT error
        ptErr1 = dau1->ptError();
        ptErr2 = dau2->ptError();

        
        //vertexCovariance 00-xError 11-y 22-z
        secvz = trk.vz(); secvx = trk.vx(); secvy = trk.vy();
        //secvzError = sqrt(trk.vertexCovariance(2,2)); secvxError = sqrt(trk.vertexCovariance(0,0)); secvyError = sqrt(trk.vertexCovariance(1,1));
        
        //trkNHits
        nhit1 = dau1->numberOfValidHits();
        nhit2 = dau2->numberOfValidHits();

        //DCA

        
        double dzbest1 = dau1->dz(bestvtx);
        double dxybest1 = dau1->dxy(bestvtx);
        double dzerror1 = sqrt(dau1->dzError()*dau1->dzError()+bestvzError*bestvzError);
        double dxyerror1 = sqrt(dau1->d0Error()*dau1->d0Error()+bestvxError*bestvyError);
    
        dzos1 = dzbest1/dzerror1;
        dxyos1 = dxybest1/dxyerror1;
        
        double dzbest2 = dau2->dz(bestvtx);
        double dxybest2 = dau2->dxy(bestvtx);
        double dzerror2 = sqrt(dau2->dzError()*dau2->dzError()+bestvzError*bestvzError);
        double dxyerror2 = sqrt(dau2->d0Error()*dau2->d0Error()+bestvxError*bestvyError);
        
        dzos2 = dzbest2/dzerror2;
        dxyos2 = dxybest2/dxyerror2;

        //vtxChi2
        vtxChi2 = trk.vertexChi2();
        ndf = trk.vertexNdof();
        VtxProb = TMath::Prob(vtxChi2,ndf);

        //PAngle
        TVector3 ptosvec(secvx-bestvx,secvy-bestvy,secvz-bestvz);
        TVector3 secvec(px,py,pz);
        
        TVector3 ptosvec2D(secvx-bestvx,secvy-bestvy,0);
        TVector3 secvec2D(px,py,0);
            
        agl = cos(secvec.Angle(ptosvec));
        agl_abs = secvec.Angle(ptosvec);

        agl2D = cos(secvec2D.Angle(ptosvec2D));
        agl2D_abs = secvec2D.Angle(ptosvec2D);

        //Decay length 3D
        typedef ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> > SMatrixSym3D;
        typedef ROOT::Math::SVector<double, 3> SVector3;
        typedef ROOT::Math::SVector<double, 6> SVector6;
        
        SMatrixSym3D totalCov = vtxPrimary->covariance() + trk.vertexCovariance();
        SVector3 distanceVector(secvx-bestvx,secvy-bestvy,secvz-bestvz);
        
        dl = ROOT::Math::Mag(distanceVector);
        dlerror = sqrt(ROOT::Math::Similarity(totalCov, distanceVector))/dl;
        
        dlos = dl/dlerror;
        
        //Decay length 2D
        SVector6 v1(vtxPrimary->covariance(0,0), vtxPrimary->covariance(0,1),vtxPrimary->covariance(1,1),0,0,0);
        SVector6 v2(trk.vertexCovariance(0,0), trk.vertexCovariance(0,1),trk.vertexCovariance(1,1),0,0,0);
        
        SMatrixSym3D sv1(v1);
        SMatrixSym3D sv2(v2);
        
        SMatrixSym3D totalCov2D = sv1 + sv2;
        SVector3 distanceVector2D(secvx-bestvx,secvy-bestvy,0);

        double dl2D = ROOT::Math::Mag(distanceVector2D);
        double dl2Derror = sqrt(ROOT::Math::Similarity(totalCov2D, distanceVector2D))/dl2D;
        
        dlos2D = dl2D/dl2Derror;
        
        //D0 DCA
        d0dca = dl*sin(agl_abs);


        //cuts for daughter tracks
        if(pt1 <= 0.7) continue;
        if(pt2 <= 0.7) continue;
        if(eta1 <= -1.5 || eta1 >= 1.5) continue;
        if(eta2 <= -1.5 || eta2 >= 1.5) continue;
        if(ptErr1/pt1 >= 0.1) continue;
        if(ptErr2/pt2 >= 0.1) continue;
        if(nhit1 < 11) continue;
        if(nhit2 < 11) continue;

        //cuts for D0 candidates
        if(pt >= 8.0 || pt < 1.5) continue;
        if(dlos <= 3.5) continue;
        if(pt >= 1.5 && pt < 2.4){
            if(agl_abs >= 0.15) continue;
            if(VtxProb <= 0.15) continue;
        }
        if(pt >= 2.4 && pt < 3.0){
            if(agl_abs >= 0.15) continue;
            if(VtxProb <= 0.13) continue;
        }
        if(pt >= 3.0 && pt < 3.5){
            if(agl_abs >= 0.12) continue;
            if(VtxProb <= 0.12) continue;
        }
        if(pt >= 3.5 && pt < 4.2){
            if(agl_abs >= 0.10) continue;
            if(VtxProb <= 0.11) continue;
        }
        if(pt >= 4.2 && pt < 5.0){
            if(agl_abs >= 0.09) continue;
            if(VtxProb <= 0.10) continue;
        }
        if(pt >= 5.0 && pt < 6.0){
            if(agl_abs >= 0.08) continue;
            if(VtxProb <= 0.09) continue;
        }
        if(pt >= 6.0 && pt < 7.0){
            if(agl_abs >= 0.07) continue;
            if(VtxProb <= 0.08) continue;
        }
        if(pt >= 7.0 && pt < 8.0){
            if(agl_abs >= 0.06) continue;
            if(VtxProb <= 0.08) continue;
        }


        //reconstucted vertex
        hpt->Fill(pt);
        heta->Fill(eta);
        hphi->Fill(phi);
        hrapidity->Fill(y);
        hmass->Fill(mass);

        for (int iPtBin = 0; iPtBin < nPtBins; iPtBin++) 
        {
            if (pt >= ptCuts[iPtBin] && pt < ptCuts[iPtBin+1]) {
                PtBins_all[iPtBin].hmass->Fill(mass); 
                break;
            }
        }


        //get trigger particles
        Particle_trg part;
        part.pt = pt;
        part.eta = eta;
        part.phi = phi;
        part.eta_dau1 = dau1->eta();
        part.phi_dau1 = dau1->phi();
        part.eta_dau2 = dau2->eta();
        part.phi_dau2 = dau2->phi();

        for (int iPtBin = 0; iPtBin < nPtBins; iPtBin++) 
        {
            if (pt >= ptCuts[iPtBin] && pt < ptCuts[iPtBin+1]) {
                for (int iMassBin = 0; iMassBin < nMassBins; iMassBin++){
                            if (mass >= massCuts[iMassBin] && mass < massCuts[iMassBin+1]){
                                particlesPerBin[iPtBin][iMassBin].push_back(part);
                                break;
                            }
                }
            }
        }
        
    }


    for (int iPtBin = 0; iPtBin < nPtBins; iPtBin++) {
        for (int iMassBin = 0; iMassBin < nMassBins; iMassBin++) {
            const auto& binParticles = particlesPerBin[iPtBin][iMassBin];
            int nTotal = binParticles.size();  

            Histograms& ptBin = PtBins_all[iPtBin];
            MassBinHistograms& h = ptBin.PtMassBins_SB[iMassBin];
            
            if (nTotal > 0) {     
                
                h.hMult_bins->Fill(nTotal);
            
                for (const auto& part_trg : binParticles) {

                    float eta_trg = part_trg.eta;
                    float phi_trg = part_trg.phi;
                    
                    for (const auto& part_ass : *pVect_ass) {  
                        float pt_ass = part_ass.pt;
                        float eta_ass = part_ass.eta;
                        float phi_ass = part_ass.phi;

                        float eff_ass = effhisto->GetBinContent(effhisto->FindBin(eta_ass,pt_ass,cent));
                        float fake_ass = fakehisto->GetBinContent(fakehisto->FindBin(eta_ass,pt_ass,cent));
                        float sec_ass = sechisto->GetBinContent(sechisto->FindBin(eta_ass,pt_ass,cent));
                        float mul_ass = mulhisto->GetBinContent(mulhisto->FindBin(eta_ass,pt_ass,cent));

                        float effweight_ass = eff_ass * (1 + mul_ass) / (1 - fake_ass) / (1 - sec_ass);
                    
                        
                        float deltaEta=eta_ass-eta_trg;
                        float deltaPhi=phi_ass-phi_trg;

                        float deltaEta_dau1 = part_trg.eta_dau1 - eta_ass;
                        float deltaPhi_dau1 = part_trg.phi_dau1 - phi_ass;
                        float deltaEta_dau2 = part_trg.eta_dau2 - eta_ass;
                        float deltaPhi_dau2 = part_trg.phi_dau2 - phi_ass;

                        if(deltaPhi>PI) deltaPhi=deltaPhi-2*PI;
                        if(deltaPhi<-PI) deltaPhi=deltaPhi+2*PI;
                        if(deltaPhi>-PI && deltaPhi<-PI/2.) deltaPhi=deltaPhi+2*PI;

                        if(deltaPhi_dau1 > PI) deltaPhi_dau1 -= 2*PI;
                        if(deltaPhi_dau1 < -PI) deltaPhi_dau1 += 2*PI;
                        if(deltaPhi_dau1 > -PI && deltaPhi_dau1 < -PI/2.) deltaPhi_dau1 += 2*PI;
                        
                        if(deltaPhi_dau2 > PI) deltaPhi_dau2 -= 2*PI;
                        if(deltaPhi_dau2 < -PI) deltaPhi_dau2 += 2*PI;
                        if(deltaPhi_dau2 > -PI && deltaPhi_dau2 < -PI/2.) deltaPhi_dau2 += 2*PI;
                        
                        //if(deltaEta==0 && deltaPhi==0) continue;
                        if(fabs(deltaEta)==0 && fabs(deltaPhi)==0) continue;
                        //if((fabs(deltaEta_dau1)==0 && fabs(deltaPhi_dau1)==0) || (fabs(deltaEta_dau2)==0 && fabs(deltaPhi_dau2)==0)) continue;
                        if((fabs(deltaEta_dau1)<tolerance && fabs(deltaPhi_dau1)<tolerance) || (fabs(deltaEta_dau2)<tolerance && fabs(deltaPhi_dau2)<tolerance)) continue;
                
                        h.hSignal->Fill(deltaEta,deltaPhi,1.0/nTotal/effweight_ass);
                    }
                }
            }
        }
        
    }

    pVectVectVect_trg->push_back(particlesPerBin);
    pVectVect_ass->push_back(*pVect_ass);
    zvtxVect->push_back(bestvz);
    nNtrkoffline->push_back(nMult_Ntrkoffline);
    ncent->push_back(cent);

    delete pVect_ass;

}
// ------------ method called once each job just before starting event loop  ------------
void OOD0Skim::beginJob() {
    edm::Service<TFileService> fs;

    TH1D::SetDefaultSumw2();
    TH2D::SetDefaultSumw2();

    edm::FileInPath fip1("mytry/DemoAnalyzer/data/Eff_OO_2025_Hijing_MB_Centrality_NoPU_3D_Nominal_Official.root");
    TFile f1(fip1.fullPath().c_str(), "READ");
    effhisto = (TH3F*)f1.Get("hEff_3D");
    effhisto->SetDirectory(nullptr); 
    fakehisto = (TH3F*)f1.Get("hFak_3D");
    fakehisto->SetDirectory(nullptr); 
    sechisto = (TH3F*)f1.Get("hSec_3D");
    sechisto->SetDirectory(nullptr); 
    mulhisto = (TH3F*)f1.Get("hMul_3D");
    mulhisto->SetDirectory(nullptr); 
    f1.Close();  

    hNtrk = fs->make<TH1D>("hNtrk", "Number of good tracks; N_{trk}; Events", 500, 0, 500);

    hpt = fs->make<TH1D>("hpt", "D^{0} transverse momentum; p_{T} (GeV/c); Entries", 200, 0, 20);
    heta = fs->make<TH1D>("heta", "D^{0} pseudorapidity; #eta; Entries", 100, -3, 3);
    hrapidity = fs->make<TH1D>("hrapidity", "D^{0} rapidity; y; Entries", 100, -2, 2);
    hphi = fs->make<TH1D>("hphi", "D^{0} azimuthal angle; #phi; Entries", 90, -PI, PI);
    hmass = fs->make<TH1D>("hmass", "D^{0} invariant mass; m_{K#pi} (GeV/c^{2}); Entries", 60, 1.7, 2.0);
    

    PtBins_all.resize(nPtBins);

    for(int iPtBin = 0; iPtBin < nPtBins; iPtBin++){
        PtBins_all[iPtBin] = Histograms{};

        TFileDirectory ptDir = fs->mkdir(Form("ptBin%d", iPtBin));

        Histograms& ptBin = PtBins_all[iPtBin];
            
            TString massName = Form("hmass_ptBin%d", iPtBin);
            TString massTitle = Form("D^{0} mass spectrum, %.1f < p_{T} < %.1f GeV/c", 
                                     ptCuts[iPtBin], ptCuts[iPtBin+1]);

            //ptBin.hmass = new TH1D(massName, massTitle, 60, 1.7, 2.0);
            ptBin.hmass = ptDir.make<TH1D>(massName, massTitle, 60, 1.7, 2.0);
            
            ptBin.PtMassBins_SB.resize(nMassBins);

            TFileDirectory massDir = ptDir.mkdir(Form("massBins_pt%d", iPtBin));
            
            for (int iMassBin = 0; iMassBin < nMassBins; iMassBin++) {
                ptBin.PtMassBins_SB[iMassBin] = MassBinHistograms{};
                MassBinHistograms& massBin = ptBin.PtMassBins_SB[iMassBin];

                TFileDirectory binDir = massDir.mkdir(Form("massBin%d", iMassBin));
                
                TString multName = Form("hMult_pt%d_mass%d", iPtBin, iMassBin);
                TString multTitle = Form("Multiplicity, ptBin%d, massBin%d", iPtBin, iMassBin);
                //massBin.hMult_bins = new TH1D(multName, multTitle, 100, 0, 100);
                massBin.hMult_bins = binDir.make<TH1D>(multName, multTitle, 100, 0, 100);
                
                TString sigName = Form("hSignal_pt%d_mass%d", iPtBin, iMassBin);
                //massBin.hSignal = new TH2D(sigName, ";#Delta#eta;#Delta#phi", 
                //                           33,-4.95,4.95,31,-(0.5-1.0/32)*PI,(1.5-1.0/32)*PI);  
                massBin.hSignal = binDir.make<TH2D>(sigName, ";#Delta#eta;#Delta#phi", 
                                           33,-4.95,4.95,31,-(0.5-1.0/32)*PI,(1.5-1.0/32)*PI);

                TString bkgName = Form("hBackground_pt%d_mass%d", iPtBin, iMassBin);
                //massBin.hBackground = new TH2D(bkgName, ";#Delta#eta;#Delta#phi",
                //                           33,-4.95,4.95,31,-(0.5-1.0/32)*PI,(1.5-1.0/32)*PI); 
                massBin.hBackground = binDir.make<TH2D>(bkgName, ";#Delta#eta;#Delta#phi",
                                           33,-4.95,4.95,31,-(0.5-1.0/32)*PI,(1.5-1.0/32)*PI); 
        
            massBin.hSignal->Sumw2();
            massBin.hBackground->Sumw2();
        }
    }


    pVectVectVect_trg = new vector< vector< vector< vector<Particle_trg> > > >;
    pVectVect_ass = new vector< vector<Particle_ass> >;
    zvtxVect = new vector<float>;
    nNtrkoffline = new vector<int>;
    ncent = new vector<int>;
}

// ------------ method called once each job just after ending the event loop  ------------
void OOD0Skim::endJob() {
  // Get total event counts
    int nevttotal_trg = (int)pVectVectVect_trg->size();
    int nevttotal_ass = (int)pVectVect_ass->size();

    // Process each pt bin
    for (int iPtBin = 0; iPtBin < nPtBins; iPtBin++){
        // Process each mass bin
        for (int iMassBin = 0; iMassBin < nMassBins; iMassBin++) 
        {
            Histograms& ptBin = PtBins_all[iPtBin];
            MassBinHistograms& h = ptBin.PtMassBins_SB[iMassBin];
        for(int nround=0; nround<20; nround++)
        {
            int ncount = 0;
            
            // Process each trigger event
            for(int nevt_trg = 0; nevt_trg < nevttotal_trg; nevt_trg++)
            {
                int nevt_ass = gRandom->Integer(nevttotal_ass);
                if(nevt_trg == nevt_ass) { nevt_trg--; continue; }
                if(fabs((*zvtxVect)[nevt_trg]-(*zvtxVect)[nevt_ass])>0.5 || 
                fabs((*nNtrkoffline)[nevt_trg] - (*nNtrkoffline)[nevt_ass]) > 10 ){
                    nevt_trg--;
                    ncount++;
                    if(ncount>5000) {nevt_trg++; ncount = 0;}
                    continue; 
                }
                    // Get particle collections
                    const auto& pVectTmp_trg = (*pVectVectVect_trg)[nevt_trg][iPtBin][iMassBin]; // Trigger particles 
                    const auto& pVectTmp_ass = (*pVectVect_ass)[nevt_ass];        // Associated particles
                    int nMult_trg = pVectTmp_trg.size();
                    
                    // Skip events with no trigger particles
                    if(nMult_trg == 0) continue;
                    
                    // Particle pairing loop
                    
                    for(const auto& part_trg : pVectTmp_trg) 
                    {                        
                        float eta_trg = part_trg.eta;
                        float phi_trg = part_trg.phi;
                        
                        for(const auto& part_ass : pVectTmp_ass) 
                        {
                            float pt_ass = part_ass.pt;
                            float eta_ass = part_ass.eta;
                            float phi_ass = part_ass.phi;

                            float eff_ass = effhisto->GetBinContent(effhisto->FindBin(eta_ass,pt_ass,(*ncent)[nevt_ass]));
                            float fake_ass = fakehisto->GetBinContent(fakehisto->FindBin(eta_ass,pt_ass,(*ncent)[nevt_ass]));
                            float sec_ass = sechisto->GetBinContent(sechisto->FindBin(eta_ass,pt_ass,(*ncent)[nevt_ass]));
                            float mul_ass = mulhisto->GetBinContent(mulhisto->FindBin(eta_ass,pt_ass,(*ncent)[nevt_ass]));

                            float effweight_ass = eff_ass * (1 + mul_ass) / (1 - fake_ass) / (1 - sec_ass);
                        
                            
                            float deltaEta=eta_ass-eta_trg;
                            float deltaPhi=phi_ass-phi_trg;

                            float deltaEta_dau1 = part_trg.eta_dau1 - eta_ass;
                            float deltaPhi_dau1 = part_trg.phi_dau1 - phi_ass;
                            float deltaEta_dau2 = part_trg.eta_dau2 - eta_ass;
                            float deltaPhi_dau2 = part_trg.phi_dau2 - phi_ass;

                            if(deltaPhi>PI) deltaPhi=deltaPhi-2*PI;
                            if(deltaPhi<-PI) deltaPhi=deltaPhi+2*PI;
                            if(deltaPhi>-PI && deltaPhi<-PI/2.) deltaPhi=deltaPhi+2*PI;

                            if(deltaPhi_dau1 > PI) deltaPhi_dau1 -= 2*PI;
                            if(deltaPhi_dau1 < -PI) deltaPhi_dau1 += 2*PI;
                            if(deltaPhi_dau1 > -PI && deltaPhi_dau1 < -PI/2.) deltaPhi_dau1 += 2*PI;
                            
                            if(deltaPhi_dau2 > PI) deltaPhi_dau2 -= 2*PI;
                            if(deltaPhi_dau2 < -PI) deltaPhi_dau2 += 2*PI;
                            if(deltaPhi_dau2 > -PI && deltaPhi_dau2 < -PI/2.) deltaPhi_dau2 += 2*PI;
                            
                            //if(deltaEta==0 && deltaPhi==0) continue;
                            if(fabs(deltaEta)==0 && fabs(deltaPhi)==0) continue;
                            //if((fabs(deltaEta_dau1)==0 && fabs(deltaPhi_dau1)==0) || (fabs(deltaEta_dau2)==0 && fabs(deltaPhi_dau2)==0)) continue;
                            if((fabs(deltaEta_dau1)<tolerance && fabs(deltaPhi_dau1)<tolerance) || (fabs(deltaEta_dau2)<tolerance && fabs(deltaPhi_dau2)<tolerance)) continue;
                            
                            // Fill background histograms
                            h.hBackground->Fill(deltaEta, deltaPhi, 1.0/nMult_trg/effweight_ass);
                        }
                    }
                }
            }
        } 
    }

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void OOD0Skim::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //edm::ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks", edm::InputTag("ctfWithMaterialTracks"));
  //descriptions.addWithDefaultLabel(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(OOD0Skim);
