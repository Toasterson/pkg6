#include <iostream>
#include <image/Image.h>
#include <boost/program_options.hpp>

using namespace std;
using namespace pkg;
namespace opt = boost::program_options;

int main(int argc, const char** argv) {
    try {


        opt::options_description global("Global Options");
        global.add_options()
                ("R", opt::value<std::string>()->required(), "Image root to modify defaults to /")
                ("help,h", "produce help output")
                ("command", opt::value<std::string>()->required(), "command to execute")
                ("subargs", opt::value<std::vector<std::string>>(), "Arguments for command");
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

        if (variables_map.empty() or variables_map.count("help")) {
            cerr << "Usage: pkg [global options] command [command options]" << endl;
            cerr << "type help command for help about command" << endl;
            cerr << global << endl;
            return false;
        }

        opt::notify(variables_map);
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
        } else if (cmd == "install") {
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
            for (auto itr = 0; itr < opts.size(); itr++) {
                std::string pkg = opts[itr];
                if (!boost::starts_with(pkg, "-")) {
                    packages.push_back(pkg);
                    opts[itr] = "";
                }
            }


            // Parse again...
            opt::store(opt::command_line_parser(opts).options(install_desc).run(), variables_map);
            /*
             * Pseudo Code
             * imgplan = Image.makePlan(packages)
             *      catalog.resolve(packages, resolve_strategy) || throw not known error
             *
             *      for package in resolved
             *          if !installed.contains(resolved_packages)
             *              tochange(package)
             *          else
             *              throw already installed error
             *          catalog.dependResolve(package)
             *              for dep in resolved.dependencies
             *                  if installed.contains(dep)
             *                      continue
             *
             *                  if imgPlan.contains(dep)
             *                      continue
             *
             *                  catalog.dependResolve(dep)
             *                  imgPlan.add(dep)
             *
             *           imgPlan.add(package)
             *
             * imgplan.intall()
             *      Downloader.getPakageManifest(imgplan)
             *      Downloader.getFiles(imgplan)
             *      for action in imgplan.files, dirs, links, drivers, services, users, groups, hardlinks, legacy
             *
             *
             */
            std::string IMAGE_ROOT = "/";
            if(variables_map.count("R")){
                IMAGE_ROOT = variables_map["R"].as<std::string>();
            }
            pkg::Image image = Image(IMAGE_ROOT);
            try {
                pkg::ImagePlan imgplan = image.makePlan(packages);
                if(image.needsUpgrade()){
                    image.upgrade_format();
                }
                imgplan.install();
            }
            catch (std::exception e){
                cerr << e.what() << endl;
                return false;
            }
        }
    }
    catch (std::exception& e){
        cerr << "Error: " << e.what() << endl;
        return false;
    }
    catch (...){
        cerr << "Unknown error!" << endl;
        return false;
    }
    return 0;
}