Float_t getSF( Float_t pt1_in , Float_t eta1_in , Float_t pt2_in , Float_t eta2_in , TH2D *mapin , Int_t Idx ){

  Float_t eta_max = 2.49;
  Float_t eta_min = -2.5;
  Float_t pt_max = 199.;
  Float_t pt_min = 15.;
    
  if ( eta1_in < eta_min ) eta1_in = eta_min;
  if ( eta1_in > eta_max ) eta1_in = eta_max;
  if ( pt1_in < pt_min   ) pt1_in  = pt_min;
  if ( pt1_in > pt_max   ) pt1_in  = pt_max;

  if ( eta2_in < eta_min ) eta2_in = eta_min;
  if ( eta2_in > eta_max ) eta2_in = eta_max;
  if ( pt2_in < pt_min   ) pt2_in  = pt_min;
  if ( pt2_in > pt_max   ) pt2_in  = pt_max;
  
  Float_t sf1 = mapin->GetBinContent( mapin->FindBin( abs(eta1_in) , pt1_in ) );
  Float_t sf2 = mapin->GetBinContent( mapin->FindBin( abs(eta2_in) , pt2_in ) );

  if ( Idx == 0 ) { 
    return sf1*sf2;
  }
  else if ( Idx == 1 ) {
    return sf1;
  }
  else if ( Idx == 2 ) {
    return sf2;
  }
  else if ( Idx == 3 ) {
    return (sf1+sf2)/2;
  }
  else {
    return 0.;
  }
}

Float_t getFlip( Float_t pt1_in , Float_t eta1_in , Float_t pt2_in , Float_t eta2_in , TH2D *mapin ) {

  Float_t eta_max = 2.49;
  Float_t eta_min = -2.5;
  Float_t pt_max = 199.;
  Float_t pt_min = 15.;

  if ( eta1_in < eta_min ) eta1_in = eta_min;
  if ( eta1_in > eta_max ) eta1_in = eta_max;
  if ( pt1_in < pt_min   ) pt1_in  = pt_min;
  if ( pt1_in > pt_max   ) pt1_in  = pt_max;

  if ( eta2_in < eta_min ) eta2_in = eta_min;
  if ( eta2_in > eta_max ) eta2_in = eta_max;
  if ( pt2_in < pt_min   ) pt2_in  = pt_min;
  if ( pt2_in > pt_max   ) pt2_in  = pt_max;
  
  Float_t flip1 = 0. ; Float_t flip2 = 0. ; Float_t commonW = 1. ;
  
  flip1 = mapin->GetBinContent( mapin->FindBin( abs(eta1_in) , pt1_in ) );
  flip2 = mapin->GetBinContent( mapin->FindBin( abs(eta2_in) , pt2_in ) );
  
  commonW = flip1 * ( 1. - flip2 ) + flip2 * ( 1. - flip1 );
  
  return commonW;
}

void process_SF( TString fin , TString fout , TString flip ) { 
  
  TFile *f = new TFile( fin , "read" );
  TTree *tree = (TTree*)f->Get("flipper");
  
  TFile *g = new TFile( fout , "recreate" );
  TTree *newtree = tree->CloneTree();
  

  TFile *load2D = new TFile( flip , "read" );
  TH2D *hsf   = (TH2D*) load2D->Get("sf");
  TH2D *hmc   = (TH2D*) load2D->Get("mc");
  TH2D *hdata = (TH2D*) load2D->Get("data");

  Float_t lep1_pt, lep2_pt, lep1_eta, lep2_eta;
  Float_t sf , sf1 , sf2 , sf3 , f_mc , f_data;

  TBranch *bSF       = newtree->Branch( "sf"     , &sf     , "sf/F"     );
  TBranch *bSF1      = newtree->Branch( "sf1"     , &sf1     , "sf1/F"     );
  TBranch *bSF2      = newtree->Branch( "sf2"     , &sf2     , "sf2/F"     );
  TBranch *bSF3      = newtree->Branch( "sf3"     , &sf3     , "sf3/F"     );
  TBranch *bflipmc   = newtree->Branch( "f_mc"   , &f_mc   , "f_mc/F"   );
  TBranch *bflipdata = newtree->Branch( "f_data" , &f_data , "f_data/F" );

  tree->SetBranchAddress( "lep1_pt"  , &lep1_pt );
  tree->SetBranchAddress( "lep2_pt"  , &lep2_pt ); 
  tree->SetBranchAddress( "lep1_eta" , &lep1_eta );
  tree->SetBranchAddress( "lep2_eta" , &lep2_eta );

  Long64_t nentries = tree->GetEntries(); 
  for (Long64_t i=0;i<nentries;i++) { 
    tree->GetEntry(i); 
    sf      = getSF( lep1_pt , lep1_eta , lep2_pt , lep2_eta , hsf , 0 ) ;
    sf1     = getSF( lep1_pt , lep1_eta , lep2_pt , lep2_eta , hsf , 1 ) ;
    sf2     = getSF( lep1_pt , lep1_eta , lep2_pt , lep2_eta , hsf , 2 ) ;
    sf3     = getSF( lep1_pt , lep1_eta , lep2_pt , lep2_eta , hsf , 3 ) ;
    f_mc    = getFlip ( lep1_pt , lep1_eta , lep2_pt , lep2_eta , hmc   ) ;
    f_data  = getFlip ( lep1_pt , lep1_eta , lep2_pt , lep2_eta , hdata ) ;
    bSF->Fill();
    bSF1->Fill();
    bSF2->Fill();
    bSF3->Fill();
    bflipmc->Fill();
    bflipdata->Fill();
  }
  load2D->Close();
  newtree->Print();
  newtree->GetCurrentFile()->Write();
  g->Close();
  delete f;
}
