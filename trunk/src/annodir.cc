/*
 * annodir -- src/annodir.cc
 * Copyright (c) 2004 Ciaran McCreesh <ciaranm at gentoo.org>
 *
 * This file is part of annodir.
 *
 * annodir is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * annodir is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * annodir; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 325, Boston, MA  02111-1257  USA
 */

#include "config.h"
#include "src/anno_db.hh"
#include "src/options.hh"
#include "src/exceptions.hh"
#include "src/action_handler.hh"
#include "src/action_list_handler.hh"
#include "src/action_add_handler.hh"
#include "src/rcfile.hh"

#include <unistd.h>
#ifdef HAVE_GETOPT_H
# include <getopt.h>
#endif /* HAVE_GETOPT_H */
#include <errno.h>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <memory>

#ifdef HAVE_GETOPT_LONG
static struct option long_options[] =
{
    /* general options */
    {"recursive",         no_argument,         0,   'R'},
      /* verbose is for extra information (dates, creator etc) and more
       * verbose error reporting */
    {"verbose",           no_argument,         0,   'v'},
      /* compact tries to fit each entry on one line */
    {"compact",           no_argument,         0,   'c'},
      /* summarise displays only the titles */
    {"summarise",         no_argument,         0,   's'},
    {"summarize",         no_argument,         0,   's'}, /* silly usians */
      /* file lets the user specify a file other than .annodir */
    {"file",              required_argument,   0,   'f'},
      /* lets the user override the created_by etc fields */
    {"user",              required_argument,   0,   'u'},

    /* actions */
    {"list",              optional_argument,   0,   'l'},
    {"add",               optional_argument,   0,   'a'},
    {"delete",            optional_argument,   0,   'd'},
    {"edit",              optional_argument,   0,   'e'},

    /* standard stuff */
    {"help",              no_argument,         0,   'h'},
    {"version",           no_argument,         0,   '\3'},
    { 0, 0, 0, 0 }
};
#endif /* HAVE_GETOPT_LONG */

static const char *short_options = "vcsRh\3f:u:a::e::l::d::";

/*
 * Display usage.
 */
    void
usage()
{
    std::cerr
        << "Usage: annodir [options]" << std::endl
        << "Try annodir "
#ifdef HAVE_GETOPT_LONG
        << "--help"
#else
        << "-h"
#endif /* HAVE_GETOPT_LONG */
        << " for further details" << std::endl;
}

/*
 * Display help.
 */
    void
help()
{
   /* TODO: finish */
    std::cerr
        << "TODO" << std::endl;
}

/*
 * Display version.
 */
    void
version()
{
    std::cout << PACKAGE << "-" << VERSION << std::endl;
}

/**
 * Handle commandline.
 */
    int
handle_options(int argc, char *argv[], options_T *opts)
{
    int key, option_index = 0;

    while (1)
    {

#ifdef HAVE_GETOPT_LONG
        key = getopt_long(argc, argv, short_options,
                long_options, &option_index);
#else
        key = getopt(argc, argv, short_options);
#endif /* HAVE_GETOPT_LONG */

        if (key == -1)
            break;

        switch (key)
        {
            case 'v': /* verbose */
                opts->set_verbose(true);
                break;

            case 'c': /* compact */
                opts->set_compact(true);
                break;

            case 's': /* summarise */
                opts->set_summarise(true);
                break;

            case 'R': /* recursive */
                opts->set_recursive(true);
                break;
            
            case 'f': /* file */
                opts->set_filename(optarg);
                break;

            case 'u': /* user */
                opts->set_user(optarg);
                break;

            case 'a': /* action add */
                if (opts->action() != action_unspecified)
                    throw args_one_action_only_E();
                opts->set_action(action_add);
                break;

            case 'e': /* action edit */
                if (opts->action() != action_unspecified)
                    throw args_one_action_only_E();
                opts->set_action(action_edit);
                break;

            case 'l': /* action list */
                if (opts->action() != action_unspecified)
                    throw args_one_action_only_E();
                opts->set_action(action_list);
                break;

            case 'd': /* action delete */
                if (opts->action() != action_unspecified)
                    throw args_one_action_only_E();
                opts->set_action(action_delete);
                break;

            case 'h': /* help */
                throw args_help_E();
                break;

            case '\3': /* version */
                throw args_version_E();
                break;

            case 0:
                throw args_usage_E();
                break;

            default:
                throw args_E();
                break;
        }
    }

    if (optind < argc)
        throw args_usage_E();

    return 0;
}

    int
main(int argc, char *argv[])
{
    options_T options;
    char *getenv_result;

    /* initialise 'unimportant' things from env */
    if ((getenv_result = getenv("USER")))
        options.set_user(getenv_result);

    /* initialise from user rcfile */
    if ((getenv_result = getenv("HOME")))
    {
        std::string rcname(getenv_result);
        rcname.append("/.annodirrc");
        std::auto_ptr<std::ifstream > f(new std::ifstream(rcname.c_str()));
        if (*f)
        {
            rcfile_T rcfile(*f);
            rcfile_keys_T::iterator pos;

            if (rcfile.keys.end() != (pos = rcfile.keys.find("user")))
                options.set_user(pos->second.c_str());

            if (rcfile.keys.end() != (pos = rcfile.keys.find("file")))
                options.set_filename(pos->second.c_str());
        }
    }

    /* initialise 'important' things from env */
    if ((getenv_result = getenv("ANNODIR_FILE")))
        options.set_filename(getenv_result);

    if ((getenv_result = getenv("ANNODIR_USER")))
        options.set_user(getenv_result);

    /* initalise from and handle commandline */
    try
    {
        int r = handle_options(argc, argv, &options);
        if (r != 0)
            throw args_E();

        if (options.action() == action_unspecified)
            options.set_action(action_list);

        std::map<options_action_T, action_handler_T * > handlers;
        handlers.insert(std::make_pair(action_add,  new action_add_handler_T) );
        handlers.insert(std::make_pair(action_list, new action_list_handler_T) );

        action_handler_T *handler = handlers[options.action()];
        if (handler)
            (*handler)();
        else
            throw args_unimplemented_E();

    }
    catch (args_help_E)
    {
        help();
        return EXIT_SUCCESS;
    }
    catch (args_version_E)
    {
        version();
        return EXIT_SUCCESS;
    }
    catch (args_one_action_only_E)
    {
        std::cerr << "Only one action should be specified" << std::endl;
        usage();
        return EXIT_FAILURE;
    }
    catch (args_usage_E)
    {
        usage();
        return EXIT_FAILURE;
    }
    catch (args_unimplemented_E)
    {
        std::cerr << "Sorry, feature not yet implemented." << std::endl;
        return EXIT_FAILURE;
    }
    catch (args_E)
    {
        usage();
        return EXIT_FAILURE;
    }
}

/* vim: set tw=80 sw=4 et : */
