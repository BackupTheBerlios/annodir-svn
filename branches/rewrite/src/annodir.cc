/*
 * annodir -- src/annodir.cc
 * $Id$
 * Copyright (c) 2005 Ciaran McCreesh <ciaranm at gentoo.org>
 * Copyright (c) 2005 Aaron Walker <ka0ttic@gentoo.org>
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_GETOPT_H
# include <getopt.h>
#endif /* HAVE_GETOPT_H */

#include "common.hh"
#include "action_list_handler.hh"

static const char *short_options = "Vvch";

#ifdef HAVE_GETOPT_LONG
static const struct option long_options[] =
{
    {"verbose",     no_argument,    0,  'v'},
    {"compact",     no_argument,    0,  'c'},
    {"version",     no_argument,    0,  'V'},
    {"help",        no_argument,    0,  'h'},
    { 0, 0, 0, 0 }
};
#endif /* HAVE_GETOPT_LONG */

/*
 * Display version information
 */
static void
version()
{
    std::cout << PACKAGE << "-" << VERSION << std::endl;
}

/*
 * Display usage information
 */

static void
usage()
{
    std::cerr
        << "Usage: annodir [options] [action] [args]" << std::endl
        << "Try annodir "
#ifdef HAVE_GETOPT_LONG
        << "--help"
#else /* HAVE_GETOPT_LONG */
        << "-h"
#endif /* HAVE_GETOPT_LONG */
        << " for further details." << std::endl;
}

/*
 * Display help information
 */

static void
help()
{
    std::cout << "TODO" << std::endl;
}

/*
 * Handle command line options
 */

static int
handle_options(int argc, char **argv, opts_type *args)
{
    int key, opt_index = 0;

    while (true)
    {
#ifdef HAVE_GETOPT_LONG
        key = getopt_long(argc, argv, short_options, long_options, &opt_index);
#else /* HAVE_GETOPT_LONG */
        key = getopt(argc, argv, short_options);
#endif /* HAVE_GETOPT_LONG */

        if (key == -1)
            break;

        switch (key)
        {
            case 'v':
                optset("verbose", bool, true);
                break;
            case 'c':
                optset("compact", bool, true);
                break;
            case 'V':
                throw args_version_E();
                break;
            case 'h':
                throw args_help_E();
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
    {
        while (optind < argc)
            args->push_back(argv[optind++]);
    }
    else
        throw args_usage_E();

    return 0;
}

int
main(int argc, char **argv)
{
    options_T options;
    opts_type nonopt_args;

    try
    {
        /* process command line arguments */
        if (handle_options(argc, argv, &nonopt_args) != 0)
            throw args_E();

        /* setup output stream */
        {
            util::string outfile(optget("outfile", util::string));
            std::ostream *outstream = NULL;

            if (outfile != "stdout" and outfile != "stderr")
            {
                outstream = new std::ofstream(outfile.c_str());
                if (not *outstream)
                    throw util::bad_fileobject_E(outfile);
            }
            else
            {
                /* save locale name */
                try
                {
                    optset("locale", util::string, std::locale("").name());
                }
                catch (const std::runtime_error)
                {
                    util::string error("Invalid locale");
                    char *result = std::getenv("LC_ALL");
                    if (result)
                        error += " '" + util::string(result) + "'.";
                    std::cerr << error << std::endl;
                    return EXIT_FAILURE;
                }
            }

            /* imbue output stream w/locale */
            optget("outstream", std::ostream *)->imbue
                (std::locale(optget("locale", util::string).c_str()));
        }

        /* dump options */
        if (optget("debug", bool))
            options.dump(*optget("outstream", std::ostream *));

        /* set default action if currently unset */
        if (optget("action", options_action_T) == action_unspecified)
            optset("action", options_action_T, action_list);

        std::map<options_action_T, action_handler_T * > handlers;
        handlers[action_list] = new action_list_handler_T();

        /* execute action handler */
        action_handler_T *handler = handlers[optget("action", options_action_T)];
        if (handler)
        {
            try
            {
                (*handler)(nonopt_args);
            }
            catch (item_E)
            {
                std::cerr << "Error reading input." << std::endl;
                return EXIT_FAILURE;
            }
        }
        else
            throw args_unimplemented_E();
    }
    catch (const args_help_E)
    {
        help();
        return EXIT_SUCCESS;
    }
    catch (const args_version_E)
    {
        version();
        return EXIT_SUCCESS;
    }
    catch (const args_one_action_only_E)
    {
        std::cerr << "Only one action should be specified." << std::endl;
        usage();
        return EXIT_FAILURE;
    }
    catch (const args_usage_E)
    {
        usage();
        return EXIT_FAILURE;
    }
    catch (const args_unimplemented_E)
    {
        std::cerr << "Feature not yet implemented." << std::endl;
        return EXIT_FAILURE;
    }
    catch (const args_E)
    {
        usage();
        return EXIT_FAILURE;
    }
    catch (const annodir_base_E &e)
    {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/* vim: set tw=80 sw=4 et : */
