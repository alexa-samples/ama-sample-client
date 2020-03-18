/*

Copyright Amazon.com, Inc. and its affiliates. All Rights Reserved.
 
SPDX-License-Identifier: LicenseRef-.amazon.com.-AmznSL-1.0

Licensed under the Amazon Software License (the "License").
You may not use this file except in compliance with the License.
A copy of the License is located at

  http://aws.amazon.com/asl/

or in the "license" file accompanying this file. This file is distributed
on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
express or implied. See the License for the specific language governing
permissions and limitations under the License.

*/

#include "audioProcessor.h"

#include <iostream> // cin
#include <fstream>
#include <thread>
#include <string>

bool isQuit = false;
void captureSound() {

  AudioProcessor ap;
  ap.open("default", 48000);

  std::ofstream outFile;
  outFile.open("MyRecord.opus", std::ios::binary | std::ios::out);

  int bufSize = ap.getMaxBufSize();
  unsigned char* buf = new unsigned char[bufSize];
  ap.setBuffer(buf);

  while(!isQuit) {
     int numBytes = ap.processAudio();
     // outFile.write((const char*) &numBytes, sizeof(int)); // Temporary
     outFile.write((const char*) buf, numBytes);
  }
  
  outFile.close();
  ap.close();
  delete [] buf;
}

int main() {
  std::string line;

  bool done = false;
  std::thread* t = nullptr;

  while (true) {
    std::getline (std::cin, line);
    if (line.size () != 1)
      continue;

    switch (line.at (0)) {
    case 'q':
      done = true;
      break;
    case 'o':
      if (t == nullptr) {
        t = new std::thread (captureSound);
      }
      break;
    case 'c':
      if (t != nullptr) {
        isQuit = true;
        t->join ();
        // delete t;
        t = nullptr;
        isQuit = false;
      }
      break;
    }
    if (done) {
      if (t != nullptr) {
        isQuit = true;
        t->join ();
        // delete t;
      }
      break;
    }
  } // end while

  return 0;
}
