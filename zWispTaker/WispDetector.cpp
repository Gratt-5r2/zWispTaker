// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  int oCPortalRoomManager::GetCurItemPortalRoomGuild_Union( oCItem* item ) {
    if( item ) {
      const zSTRING* portalName = item->GetSectorNameVobIsIn();
      if( portalName ) {
        int portalIndex = GetPortalRoomIndex( *portalName );
        if( portalIndex != Invalid ) {
          oCPortalRoom* portalRoom = portals[portalIndex];
          if( portalRoom )
            return portalRoom->GetOwnerGuild();
        }
      }
    }

    return Invalid;
  }



  bool_t CheckWispTakeItem( oCNpc* wisp, oCItem* item ) {
    oCPortalRoomManager* rooms = ogame->GetPortalRoomManager();
    if( rooms ) {
      int roomGuild        = rooms->GetCurItemPortalRoomGuild_Union( item );
      int attitude         = ogame->GetGuilds()->GetAttitude( roomGuild, player->guild );
      bool_t friendlyRoom  = attitude == NPC_ATT_FRIENDLY || roomGuild == Invalid;
      bool_t ownedByPlayer = item->IsOwnedByNpc( player->GetInstance() );
      bool_t ownedByGuild  = item->IsOwnedByGuild( player->guild );

      if( !friendlyRoom && !ownedByPlayer && !ownedByGuild )
        return False;
    }

    oCVisualFX::CreateAndPlay( "SPELLFX_SLEEP_ORIGIN", item->GetPositionWorld(), Null, 0, 0.0, 0, 0 );
    player->DoTakeVob( item );
    return True;
  }



  int AI_WispTakeItem() {
    zCParser* par  = zCParser::GetParser();
    oCItem* item   = (oCItem*)par->GetInstance();
    oCNpc* wisp    = (oCNpc*)par->GetInstance();
    bool_t canTake = CheckWispTakeItem( wisp, item );
    par->SetReturn( (int&)canTake );
    return 0;
  }
}