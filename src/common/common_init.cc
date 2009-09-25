
#include "config.h"
#include "tls.h"

#include "include/color.h"

#include "auth/KeyRing.h"

void common_init(std::vector<const char*>& args, const char *module_type, bool daemon)
{
  tls_init();
  tls_get_val()->disable_assert = 0;

  g_daemon = daemon;
  if (daemon) {
    cout << TEXT_YELLOW << " ** WARNING: Ceph is still under heavy development, and is only suitable for **" << TEXT_NORMAL << std::endl;
    cout << TEXT_YELLOW <<  " **          testing and review.  Do not trust it with important data.       **" << TEXT_NORMAL << std::endl;
    
    g_conf.daemonize = true;
    g_conf.log_to_stdout = false;
  } else {
    g_conf.daemonize = false;
    g_conf.log_to_stdout = true;
    g_conf.logger = false;
  }

  parse_startup_config_options(args, daemon, module_type);
  parse_config_options(args);

  // open log file?
  if (!g_conf.log_to_stdout)
    _dout_open_log();

  if (g_keyring.load_master(g_conf.keys_file)) {
    dout(0) << "successfuly loaded secret key from " << g_conf.keys_file << dendl;
  } else {
    dout(0) << "failed to load secret key from " << g_conf.keys_file << dendl;
  }
}

