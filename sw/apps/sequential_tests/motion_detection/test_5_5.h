// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
#define IMG_WIDTH 5
#define IMG_HEIGHT 5

__attribute__((section(".heapl2ram"))) pixel Y_back[5][5] = 
{
  { 0x89,0x89,0x89,0x88,0x88 },
  { 0x88,0x88,0x88,0x88,0x88 },
  { 0x88,0x88,0x88,0x88,0x88 },
  { 0x87,0x87,0x88,0x88,0x87 },
  { 0x87,0x88,0x87,0x87,0x88 }
};

__attribute__((section(".heapl2ram"))) pixel Y_test[5][5] = 
{
  { 0x89,0x89,0x89,0x88,0x88 },
  { 0x88,0x88,0x88,0x88,0x88 },
  { 0x88,0x88,0x88,0x88,0x88 },
  { 0x87,0x87,0x88,0x88,0x87 },
  { 0xc6,0x96,0x87,0x87,0x88 }
};

__attribute__((section(".heapl2ram"))) pixel Y_golden[5][5] = 
{
  { 0x89,0x89,0x89,0x88,0x88 },
  { 0x88,0x88,0x88,0x88,0x88 },
  { 0x88,0x0,0x0,0x88,0x88 },
  { 0x87,0x0,0x0,0x88,0x87 },
  { 0x0,0x96,0x87,0x87,0x88 }
};