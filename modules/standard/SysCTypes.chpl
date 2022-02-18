/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

/*

In previous releases, this module contained Chapel types that
represent C integer types.  It has been deprecated and its contents
have been moved to :mod:`CTypes`.

.. warning::

    This module has been deprecated - please use :mod:`CTypes` instead.
*/

deprecated "'SysCTypes' is deprecated; please use the 'CTypes' module instead"
module SysCTypes {
  public use ChapelSysCTypes;
}
