#include "hugo.h"

#ifdef NGC
extern void InitGCVideo();
extern void ResetSound();
extern void pourlogo();
extern void unpack();
extern void MainMenu ();
int hugoromsize;
unsigned char *hugorom;
#endif
//! name of the backup ram filename
static char backup_mem[PATH_MAX];

struct host_machine host;
struct hugo_options option;

//! Setup the host machine
/*!
 * \param argc The number of argument on command line
 * \param argv The arguments on command line
 * \return zero on success else non zero value
 */
int
initialisation (int argc, char *argv[])
{

#ifdef MSDOS
  _crt0_startup_flags |= _CRT0_FLAG_NO_LFN;
  // Disable long filename to avoid mem waste in select_rom func.
#endif

  memset (&option, 0, sizeof (option));

  option.want_stereo = FALSE;
  option.want_fullscreen = FALSE;
  option.want_fullscreen_aspect = FALSE;
  option.configure_joypads = FALSE;
  option.want_hardware_scaling = FALSE;
  option.want_arcade_card_emulation = TRUE; 
  option.want_supergraphx_emulation = TRUE;
  option.window_size = 1;

#if defined(ENABLE_NETPLAY)
  option.local_input_mapping[0] = 0;
  option.local_input_mapping[1] = 1;
  option.local_input_mapping[2] = -1;
  option.local_input_mapping[3] = -1;
  option.local_input_mapping[4] = -1;
  strcpy(option.server_hostname,"localhost");
#endif

  /*
   * Most sound cards won't like this value but we'll accept whatever SDL
   * gives us.  This frequency will get us really close to the original
   * pc engine sound behaviour.
   */
  option.want_snd_freq = 21963;

  // Initialise paths
  osd_init_paths (argc, argv);

  // Create the log file
  init_log_file ();

  // Init the random seed
  srand ((unsigned int) time (NULL));

#ifndef NGC
  // Read configuration in ini file
  parse_INIfile ();

  // Read the command line
  parse_commandline (argc, argv);
#endif

  // Initialise the host machine
  if (!osd_init_machine ())
    return -1;

  // If backup memory name hasn't been overriden on command line, use the default
  if ((bmdefault) && (strcmp (bmdefault, "")))
    snprintf (backup_mem, sizeof (backup_mem), "%s%s", short_exe_name,
	      bmdefault);
  else
    snprintf (backup_mem, sizeof (backup_mem), "%sbackup.dat",
	      short_exe_name);

  // In case of crash, try to free audio related ressources if possible
//      atexit (TrashSound);

  // Initialise the input devices
  if (osd_init_input () != 0)
    {
      fprintf (stderr, "Initialization of input system failed\n");
      return (-2);
    }

  return 0;
}


//! Free ressources of the host machine
/*!
 * Deallocate ressources reserved during the initialisation
 */
void
cleanup ()
{

  osd_shutdown_input();

  // Deinitialise the host machine
  osd_shut_machine ();

}

//! Check if a game was asked
/*!
 * \return non zero if a game must be played
 */
int
game_asked ()
{
  return ((CD_emulation == 1) || (strcmp (cart_name, "")));
}

//! Run an instance of a rom or cd or iso
/*!
 * \return non zero if another game has to be launched
 */
int
play_game (void)
{

#ifdef NGC
  cart_reload = 0;
  ResetSound();
#endif

  // Initialise the target machine (pce)
  if (InitPCE (cart_name, backup_mem) != 0)
    return 0;

  if (!(*osd_gfx_driver_list[video_driver].init) ())
    {
#ifndef NGC
	Log ("Can't set graphic mode\n");
      printf (MESSAGE[language][cant_set_gmode]);
#endif
      return 0;
    }

  if (!osd_snd_init_sound ())
    {
#ifndef NGC
      Log ("Couldn't open any sound hardware on the host machine.\n");
      printf (MESSAGE[language][audio_init_failed]);
#endif
    }
  else
    printf (MESSAGE[language][audio_inited], 8, "SDL compatible soundcard",
	    host.sound.freq);

#if defined(ENABLE_NETPLAY)
	osd_init_netplay();
#endif
  RunPCE ();

#if defined(ENABLE_NETPLAY)
   osd_shutdown_netplay();
#endif

  osd_snd_trash_sound ();

  (*osd_gfx_driver_list[video_driver].shut) ();

  // Free the target machine (pce)
  TrashPCE (backup_mem);

  return cart_reload;
}

int
main (int argc, char *argv[])
{
  int error = 0;

#ifdef NGC
  InitGCVideo();
  unpack();
  pourlogo();
  
  /*** Allocate cart_rom here ***/
  hugorom = malloc(2621440 + 32);
  if ((unsigned int)hugorom & 0x1f) hugorom += 32 - ( (unsigned int)hugorom & 0x1f );
  memset(hugorom, 0, 2621440);
  hugoromsize = 0;
#endif

  error = initialisation (argc, argv);

  while ( hugoromsize == 0 ) MainMenu();
  cart_reload = 0;

#if defined(GTK)

  if (!error)
    {
      if (game_asked ())
	{
	  while (play_game ());
	}
      else
	{
	  build_gtk_interface (argc, argv);
	}
    }

#else // not defined(GTK)

  if (!error)
    {
      if (game_asked () == 0 )
	{
	  while (play_game ());
	}
      else
	{
	  printf ("No game specified\n");
	}
    }
#endif

  cleanup ();

  return 0;
}
