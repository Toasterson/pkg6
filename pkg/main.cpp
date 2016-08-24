#include <iostream>
#include <image/Image.h>
#include <boost/program_options.hpp>

using namespace std;
using namespace pkg;
namespace opt = boost::program_options;

int main(int argc, const char** argv) {
    opt::options_description global("Global Options");
    global.add_options()
            ("debug", "Turn on debug output")
            ("command", opt::value<std::string>(), "command to execute")
            ("subargs", opt::value<std::vector<std::string>>(), "Arguments for command")
            ;
    opt::positional_options_description pos;
    pos.add("command", 1).
        add("subargs", -1);

    opt::variables_map variables_map;

    opt::parsed_options parsed = opt::command_line_parser(argc, argv).
        options(global).
            positional(pos).
            allow_unregistered().
            run();

    opt::store(parsed, variables_map);

    if(variables_map.empty()){
        cerr << "Usage: pkg [global options] command [command options]" << endl;
        cerr << "type help command for help about command" << endl;
        variables_map.notify();
        return 1;
    }
    std::string cmd = variables_map["command"].as<std::string>();
    if (cmd == "update") {
        // update command has the following options:
        opt::options_description update_desc("update options");
        update_desc.add_options()
                ("be-name", opt::value<std::string>(), "Name of the boot environment");

        // Collect all the unrecognized options from the first pass. This will include the
        // (positional) command name, so we need to erase that.
        std::vector<std::string> opts = opt::collect_unrecognized(parsed.options, opt::include_positional);
        opts.erase(opts.begin());

        // Parse again...
        opt::store(opt::command_line_parser(opts).options(update_desc).run(), variables_map);

        cout << "Not yet doing anything" << endl;
    } else if(cmd == "install"){
        // install command has the following options:
        opt::options_description install_desc("install options");
        install_desc.add_options()
                ("v", "verbose output");

        // Collect all the unrecognized options from the first pass. This will include the
        // (positional) command name, so we need to erase that.
        std::vector<std::string> opts = opt::collect_unrecognized(parsed.options, opt::include_positional);
        opts.erase(opts.begin());

        // Store everyting that does not have - at the begining as packages
        std::vector<std::string> packages;
        for(auto itr = 0; itr < opts.size(); itr++){
            std::string pkg = opts[itr];
            if(!boost::starts_with(pkg, "-")){
                packages.push_back(pkg);
                opts[itr] = "";
            }
        }


        // Parse again...
        opt::store(opt::command_line_parser(opts).options(install_desc).run(), variables_map);
        cout << "The Following packages where selected for installation" << endl;
        for(auto pkg : packages){
            /*
             * Basic Plan
             * Image.install(packages)
             *      resolved_packages = known.resolve(packages) || not found error
             *      tochange_pkgs
             *      for package in resolved
             *          if !installed.contains(resolved_packages)
             *              tochange(package)
             *          else
             *              throw already installed error
             *
             *      catalog.resolve_deps(tochange_pkgs) || resolve error
             *
             *      Downloader.getPakageMetadata(tochange_pkgs)
             *      Downloader.getFiles(tochange_pkgs)
             *      this->install_actions(pkg) || install error
             *          in each action files, dirs, hardlinks, symlinks, driver, service, call install
             *
             */
        }
    } else {
        cout << "Here will be some help for you guys." << endl;
    }

    return 0;
}