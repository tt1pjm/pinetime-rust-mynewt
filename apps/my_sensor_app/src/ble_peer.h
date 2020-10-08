/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
//  BLE Peer GATT Service Discovery Functions. Based on https://github.com/apache/mynewt-nimble/blob/master/apps/blecent/src/blecent.h

#ifndef H_BLEblePEER_
#define H_BLEblePEER_

#include "os/mynewt.h"
#include "modlog/modlog.h"
#ifdef __cplusplus
extern "C" {
#endif

struct ble_hs_adv_fields;
struct ble_gap_conn_desc;
struct ble_hs_cfg;
union ble_store_value;
union ble_store_key;

struct blepeer_dsc {
    SLIST_ENTRY(blepeer_dsc) next;
    struct ble_gatt_dsc dsc;
};
SLIST_HEAD(blepeer_dsc_list, blepeer_dsc);

struct blepeer_chr {
    SLIST_ENTRY(blepeer_chr) next;
    struct ble_gatt_chr chr;

    struct blepeer_dsc_list dscs;
};
SLIST_HEAD(blepeer_chr_list, blepeer_chr);

struct blepeer_svc {
    SLIST_ENTRY(blepeer_svc) next;
    struct ble_gatt_svc svc;

    struct blepeer_chr_list chrs;
};
SLIST_HEAD(blepeer_svc_list, blepeer_svc);

struct blepeer;
typedef void blepeer_disc_fn(const struct blepeer *peer, int status, void *arg);

struct blepeer {
    SLIST_ENTRY(blepeer) next;

    uint16_t conn_handle;

    /** List of discovered GATT services. */
    struct blepeer_svc_list svcs;

    /** Keeps track of where we are in the service discovery process. */
    uint16_t disc_prev_chr_val;
    struct blepeer_svc *cur_svc;

    /** Callback that gets executed when service discovery completes. */
    blepeer_disc_fn *disc_cb;
    void *disc_cb_arg;
};

int blepeer_disc_all(uint16_t conn_handle, blepeer_disc_fn *disc_cb,
                  void *disc_cb_arg);
const struct blepeer_dsc *
blepeer_dsc_find_uuid(const struct blepeer *peer, const ble_uuid_t *svc_uuid,
                   const ble_uuid_t *chr_uuid, const ble_uuid_t *dsc_uuid);
const struct blepeer_chr *
blepeer_chr_find_uuid(const struct blepeer *peer, const ble_uuid_t *svc_uuid,
                   const ble_uuid_t *chr_uuid);
const struct blepeer_svc *
blepeer_svc_find_uuid(const struct blepeer *peer, const ble_uuid_t *uuid);
int blepeer_delete(uint16_t conn_handle);
int blepeer_add(uint16_t conn_handle);
int blepeer_init(void);

#ifdef __cplusplus
}
#endif

#endif