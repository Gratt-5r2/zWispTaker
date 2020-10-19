// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  // public: class oCItem * __thiscall oCNpc::DetectItem(int,int)
  oCItem* oCNpc::DetectNoDroppedItem_Union( int flags, int best, int ignoreDropped ) {
    oCItem* bestItem = Null;
    float bestDistance = FLT_MAX;

    for( int_t i = 0; i < vobList.GetNum(); i++ ) {
      oCItem* item = vobList[i]->CastTo<oCItem>();
      if( !item )
        continue;

      // Ignore dropped items
      if( ignoreDropped && item->flags & ITM_FLAG_DROPPED )
        continue;

      // Ignore nofocus items
      if( item->flags & ITM_FLAG_NFOCUS )
        continue;

      // Ignore shit items
      if( ((item->mainflag | item->flags) & flags) == None )
        continue;

      if( !best ) {
        float distance = GetDistanceToVob( *item );
        if( distance < bestDistance ) {
          bestDistance = distance;
          bestItem = item;
        }
      }
      else if( !bestItem || item->damageTotal > bestItem->damageTotal )
        bestItem = item;
    }

    return bestItem;
  }



  int Wld_DetectItem() {
    zCParser* par = zCParser::GetParser();

    oCNpc* npc;
    int flags;
    int best;
    int ignoreDropped;
    oCItem* detectedItem;

    par->GetParameter( flags );
    npc = (oCNpc*)par->GetInstance();

    int wisp_index = par->GetIndex( "WISP_DETECTOR" );
    ignoreDropped = wisp_index == npc->instanz;
    best = !ignoreDropped;

    detectedItem = npc->DetectNoDroppedItem_Union( flags, best, ignoreDropped );
    par->SetInstance( "ITEM", detectedItem );
    par->SetReturn( detectedItem != Null );
    return 0;
  }



  HOOK Hook_Wld_DetectItem AS( 0x006E0E40, &Wld_DetectItem );
}