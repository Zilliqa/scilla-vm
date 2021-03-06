/*
 * Copyright (C) 2020 Zilliqa
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>

#include "ScillaRTL/DLog.h"
#include "ScillaRTL/Errors.h"
#include "ScillaRTL/ScillaExec.h"
#include "ScillaRTL/Utils.h"

using namespace ScillaRTL;

namespace {

namespace po = boost::program_options;

void parseCLIArgs(int argc, char *argv[], po::variables_map &VM) {
  auto UsageString = "Usage: " + std::string(argv[0]) +
                     " -g gaslimit [option...] input-file" +
                     "\nSupported options";
  po::options_description Desc(UsageString);

  // clang-format off
  Desc.add_options()
    ("output-file,o", po::value<std::string>(), "Specify output filename")
    ("gaslimit,g", po::value<uint64_t>(), "Gas limit")
    ("help,h", "Print help message")
    ("dlog", "Enable full logging (debug builds only)")
    ("dlog-only",
         po::value<std::vector<std::string> >(),
         "SRTL_DLOG_TYPE to activate for logging (debug builds only")
    ("version,v", "Print version")
  ;

  po::options_description Hidden("Hidden options");
  Hidden.add_options()
    ("input-file", po::value<std::string>(), "Specify input file")
  ;
  // clang-format on

  // Gather all options.
  po::options_description AllOptions;
  AllOptions.add(Desc).add(Hidden);

  // Mark "input-file" as a positional argument.
  po::positional_options_description P;
  P.add("input-file", 1);
  try {
    po::store(po::command_line_parser(argc, argv)
                  .options(AllOptions)
                  .positional(P)
                  .run(),
              VM);
    po::notify(VM);
  } catch (std::exception &e) {
    std::cerr << e.what() << "\n";
    std::cerr << Desc << "\n";
    exit(EXIT_FAILURE);
  }

  if (VM.count("help")) {
    std::cerr << Desc << "\n";
    exit(EXIT_SUCCESS);
  }

  // Ensure that an input file is provided.
  if (!VM.count("input-file")) {
    std::cerr << "No input file provided\n" << Desc << "\n";
    exit(EXIT_FAILURE);
  }

  // Ensure gas limit provided.
  if (!VM.count("gaslimit")) {
    std::cerr << "No gaslimit provided\n" << Desc << "\n";
    exit(EXIT_FAILURE);
  }
}

} // end of anonymous namespace

int main(int argc, char *argv[]) {

  po::variables_map VM;
  parseCLIArgs(argc, argv, VM);

  if (VM.count("dlog")) {
    ScillaRTL::enableAllDLogTypes();
  } else if (VM.count("dlog-only")) {
    auto &DTs = VM["dlog-only"].as<std::vector<std::string>>();
    for (auto &DT : DTs) {
      ScillaRTL::addToCurrentDLogTypes(DT);
    }
  }

  std::string ScillaOutput;
  try {
    auto InputFilename = VM["input-file"].as<std::string>();
    // Tool to compile the LLVM-IR to a binary shared object.
    CompileLLVMToTempSO CSO(InputFilename);

    auto GasLimit = VM["gaslimit"].as<uint64_t>();
    ScillaExprExec SJ(ScillaParams(), CSO.compile());
    ScillaOutput = SJ.exec(GasLimit);
  } catch (const ScillaError &e) {
    std::cerr << e.toString() << "\n";
    return EXIT_FAILURE;
  }

  if (VM.count("output-file")) {
    auto OutputFilename = VM["output-file"].as<std::string>();
    std::ofstream OFile(OutputFilename);
    if (!OFile) {
      std::cerr << "Error opening output file " << OutputFilename << "\n";
      return EXIT_FAILURE;
    } else {
      OFile << ScillaOutput;
      if (OFile.bad()) {
        std::cerr << "Error writing to output file " << OutputFilename << "\n";
        return EXIT_FAILURE;
      }
    }
  } else {
    std::cout << ScillaOutput;
  }

  return EXIT_SUCCESS;
}
