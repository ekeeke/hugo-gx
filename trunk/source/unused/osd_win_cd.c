#include "osd_win_cd.h"

#if defined(ALLEGRO)

#include <windows.h>
#include "myaspi32.h"

static int HaId;

#pragma pack(1)


#pragma pack()

	/*
		
		{
			int i;
			Log("First track: %d\nLast track: %d\n", toc.firstTrack, toc.lastTrack);
			
			for (i = toc.firstTrack; i <= toc.lastTrack; i++) {
			  Log("Track %d: %d\n", i, toc.tracks[i-1].addr);			
			}
		}
		
	
  /*
  */
{
 int HaId_in, Target_in, Lun_in;
 int nb = atoi(device);
 Log("Init win aspi cdrom device\n");
 if (nb == 0)
   nb = 1;
/*
 if (loadASPI())
   return 1;
*/
 Log("Trying to check cd driver number %d\n", nb);
 loadASPI();
		  
		  ReadToc();
		  
	
	printf("Reading sector %d in %s - %s\n", sector, __FILE__, __LINE__);
	
	return;
	
	/*
			  free(intern_buffer);
		  }
		  
		  free(intern_buffer);
		  
	
 Log("Looking for track info of #%d - %s %d\n", track, __FILE__, __LINE__);
	
 track--;
	

void osd_cd_subchannel_info(unsigned short offset)
{
  Wr6502(offset, 4);
  }


void osd_cd_status(int *status)
{
      *status = 0;
}

void osd_cd_pause(void)
{
}


void osd_cd_resume(void)
{
}


#include <windows.h>
#include <stdio.h>
#include "akrip32.h"

void MSB2DWORD( DWORD *d, BYTE *b );
LPTRACKBUF NewTrackBuf( DWORD numFrames );

static LPTRACKBUF track_buffer;
static HCDROM handle_CD;
static TOC toc;

int akrip_read(HCDROM hCD, unsigned int sector)
{
  DWORD dwStatus;

  track_buffer->numFrames = 1;
  track_buffer->startOffset = 0;
  track_buffer->len = 0;
  track_buffer->startFrame = sector;
  dwStatus = ReadCDAudioLBA( hCD, track_buffer );

  if ( dwStatus == SS_COMP )
    {
      return 0;
    }
  else
    {
      Log( "Error (%d:%d)\n", GetAspiLibError(), GetAspiLibAspiError() );
      return GetAspiLibAspiError();
    }

}

int init_akrip()
{
  static CDLIST cdlist;
  GETCDHAND cdh;
  HCDROM hCD;

  int cd_index, track_index, prefered_cd_index, read_toc_result;
	
			ZeroMemory( &cdlist, sizeof(cdlist) );
		
			cdlist.max = MAXCDLIST;
		
			// Check the available cd drives
			GetCDList( &cdlist );
  if ( cdlist.num < 1)
    {
      Log("No cd drive available\n");
      return 1;
    }

  track_buffer = NewTrackBuf( 1 );

  for ( cd_index = 0; (cd_index < cdlist.num) && (prefered_cd_index == -1); cd_index++ )
    {
      ZeroMemory( &cdh, sizeof(cdh) );
      cdh.size     = sizeof(GETCDHAND);
      cdh.ver      = 1;
      cdh.ha       = cdlist.cd[cd_index].ha;
      cdh.tgt      = cdlist.cd[cd_index].tgt;
      cdh.lun      = cdlist.cd[cd_index].lun;
      cdh.readType  = CDR_ANY;      // set for autodetect

      hCD = GetCDHandle( &cdh);

      ModifyCDParms( hCD, CDP_MSF, FALSE );
      if ( read_toc_result = ReadTOC( hCD, &toc ) != SS_COMP )
        {
          Log( "Error reading TOC (%d)\n", read_toc_result );
    	  continue;
#if defined(CD_DEBUG)		
      for (track_index = 0; track_index <= toc.lastTrack - toc.firstTrack; track_index++)
        {		  
          if (toc.tracks[track_index].ADR & 0X04)
            {
	          DWORD addr;
 	          MSB2DWORD( &addr, toc.tracks[track_index].addr );
#if defined(CD_DEBUG)		
              akrip_read(hCD, addr);
	          // Add detection of signature for easiness of use

	          prefered_cd_index = cd_index;
            }
        }
#if defined(CD_DEBUG)
      CloseCDHandle ( hCD );

    }  

    if (prefered_cd_index != -1)
      {

        ZeroMemory( &cdh, sizeof(cdh) );
        cdh.size     = sizeof(GETCDHAND);
        cdh.ver      = 1;
        cdh.ha       = cdlist.cd[prefered_cd_index].ha;
        cdh.tgt      = cdlist.cd[prefered_cd_index].tgt;
        cdh.lun      = cdlist.cd[prefered_cd_index].lun;
        cdh.readType  = CDR_ANY;      // set for autodetect
#if defined(CD_DEBUG)		
        handle_CD = GetCDHandle( &cdh );

        ModifyCDParms( handle_CD, CDP_MSF, FALSE );
        if ( read_toc_result = ReadTOC( handle_CD, &toc ) != SS_COMP )
          {
            Log( "Error reading TOC (%d)\n", read_toc_result);
            CloseCDHandle( handle_CD );
    	    return 1;
          }				
	      return (handle_CD >= 0 ? 0 : 1);
      }

    return 2;
}

void shut_akrip()
{
  CloseCDHandle ( handle_CD );  
}

int osd_cd_init( char* dummy )
{
  return init_akrip();
}

void osd_cd_close()
	shut_akrip();
  if (akrip_read( handle_CD, sector ) == 0)
		{
		}
	Log("Error reading cd sector %d at %s - %d\n", sector, __FILE__, __LINE__);
  return;
	


void MSB2DWORD( DWORD *d, BYTE *b )
{
  DWORD retVal;

  retVal = (DWORD)b[0];
  retVal = (retVal<<8) + (DWORD)b[1];
  retVal = (retVal<<8) + (DWORD)b[2];
  retVal = (retVal<<8) + (DWORD)b[3];

  *d = retVal;
}

void osd_cd_nb_tracks(int* first,

void osd_cd_track_info( UChar track,
#if defined(CD_DEBUG)			
 Log("Looking for track info of #%d - %s %d\n", track, __FILE__, __LINE__);
#endif
 track -= toc.firstTrack;


LPTRACKBUF NewTrackBuf( DWORD numFrames )
{
  LPTRACKBUF t;
  int numAlloc;

  numAlloc = (((int)numFrames)*2352) + TRACKBUFEXTRA;

  t = (LPTRACKBUF)GlobalAlloc( GPTR, numAlloc );

  if ( !t )
    return NULL;

  t->startFrame = 0;
  t->numFrames = 0;
  t->maxLen = numFrames * 2352;
  t->len = 0;
  t->status = 0;
  t->startOffset = 0;

  return t;
}

void osd_cd_length(int* min,
                   int* sec,
                   int* fra)
{

void osd_cd_play_audio_track(UChar track)
{

void osd_cd_play_audio_range(
			     UChar min_from,
			     UChar sec_from,
			     UChar fra_from,
			     UChar min_to,
			     UChar sec_to,
			     UChar fra_to)
{

void osd_cd_subchannel_info(unsigned short offset)
{
      Wr6502(offset, 4);
}


void osd_cd_status(int *status)
{
      *status = 0;
}

void osd_cd_pause(void)
{

void osd_cd_resume(void)
{

void osd_cd_stop_audio()
#endif