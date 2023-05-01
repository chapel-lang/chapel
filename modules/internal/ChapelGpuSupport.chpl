/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

module ChapelGpuSupport {
  use ChapelBase;
  use ChplConfig;

  extern var chpl_gpu_debug : bool;

  config const debugGpu = false;

  /* If true, upon startup, enables peer-to-peer access between all pairs of
     GPUs that are eligible for peer-to-peer access within each locale. */
  pragma "no doc"
  config const enableGpuP2P = false;

  // by virtue of module initialization:
  chpl_gpu_debug = debugGpu;

  if CHPL_LOCALE_MODEL == 'gpu' then
    if(enableGpuP2P) {
      use GPU;
      for loc in Locales do on loc do
        for gpu1 in here.gpus do
          for gpu2 in here.gpus do
            if canAccessPeer(gpu1,gpu2) then setPeerAccess(gpu1,gpu2,true);
    }
}
