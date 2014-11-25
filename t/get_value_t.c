#include "maxminddb_test_helper.h"

void test_array_0_result(int status, MMDB_entry_data_s entry_data,
                         char *function)
{
    cmp_ok(status, "==", MMDB_SUCCESS,
           "status for %s() is MMDB_SUCCESS - array[0]", function);
    ok(entry_data.has_data, "found a value for array[0]");
    cmp_ok(entry_data.type, "==", MMDB_DATA_TYPE_UINT32,
           "returned entry type is uint32 - array[0]");
    cmp_ok(entry_data.uint32, "==", 1, "entry value is 1 - array[0]");
}

void test_array_2_result(int status, MMDB_entry_data_s entry_data,
                         char *function)
{
    cmp_ok(status, "==", MMDB_SUCCESS,
           "status for %s() is MMDB_SUCCESS - array[2]", function);
    ok(entry_data.has_data, "found a value for array[2]");
    cmp_ok(entry_data.type, "==", MMDB_DATA_TYPE_UINT32,
           "returned entry type is uint32 - array[2]");
    cmp_ok(entry_data.uint32, "==", 3, "entry value is 3 - array[2]");
}

int call_vget_value(MMDB_entry_s *entry, MMDB_entry_data_s *entry_data, ...)
{
    va_list keys;
    va_start(keys, entry_data);

    int status = MMDB_vget_value(entry, entry_data, keys);

    va_end(keys);

    return status;
}

void test_simple_structure(int mode, const char *mode_desc)
{
    const char *filename = "MaxMind-DB-test-decoder.mmdb";
    const char *path = test_database_path(filename);
    MMDB_s *mmdb = open_ok(path, mode, mode_desc);
    free((void *)path);

    const char *ip = "1.1.1.1";
    MMDB_lookup_result_s result =
        lookup_string_ok(mmdb, ip, filename, mode_desc);

    {
        MMDB_entry_data_s entry_data;
        const char *lookup_path[] = { "array", "0", NULL };
        int status = MMDB_aget_value(&result.entry, &entry_data, lookup_path);
        test_array_0_result(status, entry_data, "MMDB_aget_value");

        status = MMDB_get_value(&result.entry, &entry_data, "array", "0", NULL);
        test_array_0_result(status, entry_data, "MMDB_get_value");

        status = call_vget_value(&result.entry, &entry_data, "array", "0", NULL);
        test_array_0_result(status, entry_data, "MMDB_vget_value");
    }

    {
        MMDB_entry_data_s entry_data;
        const char *lookup_path[] = { "array", "2", NULL };
        int status = MMDB_aget_value(&result.entry, &entry_data, lookup_path);
        test_array_2_result(status, entry_data, "MMDB_aget_value");

        status = MMDB_get_value(&result.entry, &entry_data, "array", "2", NULL);
        test_array_2_result(status, entry_data, "MMDB_get_value");

        status = call_vget_value(&result.entry, &entry_data, "array", "2", NULL);
        test_array_2_result(status, entry_data, "MMDB_vget_value");
    }

    MMDB_close(mmdb);
    free(mmdb);
}

void test_complex_map_a_result(int status, MMDB_entry_data_s entry_data,
                               char *function)
{
    cmp_ok(status, "==", MMDB_SUCCESS,
           "status for %s() is MMDB_SUCCESS - map1{map2}{array}[0]{map3}{a}",
           function);
    ok(entry_data.has_data,
       "found a value for map1{map2}{array}[0]{map3}{a}");
    cmp_ok(entry_data.type, "==", MMDB_DATA_TYPE_UINT32,
           "returned entry type is uint32 - map1{map2}{array}[0]{map3}{a}");
    cmp_ok(entry_data.uint32, "==", 1,
           "entry value is 1 - map1{map2}{array}[0]{map3}{a}");
}

void test_complex_map_c_result(int status, MMDB_entry_data_s entry_data,
                               char *function)
{
    cmp_ok(
        status, "==", MMDB_SUCCESS,
        "status for %s() is MMDB_SUCCESS - map1{map2}{array}[0]{map3}{c}",
        function);
    ok(entry_data.has_data,
       "found a value for map1{map2}{array}[0]{map3}{c}");
    cmp_ok(entry_data.type, "==", MMDB_DATA_TYPE_UINT32,
           "returned entry type is uint32 - map1{map2}{array}[0]{map3}{c}");
    cmp_ok(entry_data.uint32, "==", 3,
           "entry value is 3 - map1{map2}{array}[0]{map3}{c}");
}

void test_no_result(int status, MMDB_entry_data_s entry_data, char *function,
                    char *path_description)
{
    cmp_ok(status, "==", MMDB_LOOKUP_PATH_DOES_NOT_MATCH_DATA_ERROR,
           "status for %s() is MMDB_LOOKUP_PATH_DOES_NOT_MATCH_DATA_ERROR - %s",
           function, path_description);
    ok(!entry_data.has_data, "did not find a value for %s", path_description);
}

void test_nested_structure(int mode, const char *mode_desc)
{
    const char *filename = "MaxMind-DB-test-nested.mmdb";
    const char *path = test_database_path(filename);
    MMDB_s *mmdb = open_ok(path, mode, mode_desc);
    free((void *)path);

    const char *ip = "1.1.1.1";
    MMDB_lookup_result_s result =
        lookup_string_ok(mmdb, ip, filename, mode_desc);

    {
        MMDB_entry_data_s entry_data;
        const char *lookup_path[] =
        { "map1", "map2", "array", "0", "map3", "a", NULL };
        int status = MMDB_aget_value(&result.entry, &entry_data, lookup_path);
        test_complex_map_a_result(status, entry_data, "MMDB_aget_value");

        status = MMDB_get_value(&result.entry, &entry_data,
                                "map1", "map2", "array", "0", "map3", "a", NULL);
        test_complex_map_a_result(status, entry_data, "MMDB_get_value");

        status = call_vget_value(&result.entry, &entry_data,
                                 "map1", "map2", "array", "0", "map3", "a",
                                 NULL);
        test_complex_map_a_result(status, entry_data, "MMDB_vget_value");
    }

    {
        MMDB_entry_data_s entry_data;
        const char *lookup_path[] =
        { "map1", "map2", "array", "0", "map3", "c", NULL };
        int status = MMDB_aget_value(&result.entry, &entry_data, lookup_path);
        test_complex_map_c_result(status, entry_data, "MMDB_aget_value");

        status = MMDB_get_value(&result.entry, &entry_data,
                                "map1", "map2", "array", "0", "map3", "c", NULL);
        test_complex_map_c_result(status, entry_data, "MMDB_get_value");

        status = call_vget_value(&result.entry, &entry_data,
                                 "map1", "map2", "array", "0", "map3", "c",
                                 NULL);
        test_complex_map_c_result(status, entry_data, "MMDB_vget_value");
    }

    {
        MMDB_entry_data_s entry_data;
        const char *lookup_path[] =
        { "map1", "map42", "array", "0", "map3", "c", NULL };
        int status = MMDB_aget_value(&result.entry, &entry_data, lookup_path);
        test_no_result(status, entry_data, "MMDB_aget_value",
                       "map1{map42}{array}[0]{map3}{c}");

        status = MMDB_get_value(&result.entry, &entry_data,
                                "map1", "map42", "array", "0", "map3", "c",
                                NULL);
        test_no_result(status, entry_data, "MMDB_get_value",
                       "map1{map42}{array}[0]{map3}{c}");

        status = call_vget_value(&result.entry, &entry_data,
                                 "map1", "map42", "array", "0", "map3", "c",
                                 NULL);
        test_no_result(status, entry_data, "MMDB_vget_value",
                       "map1{map42}{array}[0]{map3}{c}");
    }

    {
        MMDB_entry_data_s entry_data;
        const char *lookup_path[] =
        { "map1", "map2", "array", "9", "map3", "c", NULL };
        int status = MMDB_aget_value(&result.entry, &entry_data, lookup_path);
        test_no_result(status, entry_data, "MMDB_aget_value",
                       "map1{map42}{array}[9]{map3}{c}");

        status = MMDB_get_value(&result.entry, &entry_data,
                                "map1", "map2", "array", "9", "map3", "c",
                                NULL);
        test_no_result(status, entry_data, "MMDB_get_value",
                       "map1{map42}{array}[9]{map3}{c}");

        status = call_vget_value(&result.entry, &entry_data,
                                 "map1", "map2", "array", "9", "map3", "c",
                                 NULL);
        test_no_result(status, entry_data, "MMDB_vget_value",
                       "map1{map42}{array}[9]{map3}{c}");
    }

    MMDB_close(mmdb);
    free(mmdb);
}

void test_path_too_long(int mode, const char *mode_desc)
{
    const char *filename = "MaxMind-DB-test-decoder.mmdb";
    const char *path = test_database_path(filename);
    MMDB_s *mmdb = open_ok(path, mode, mode_desc);
    free((void *)path);

    const char *ip = "1.1.1.1";
    MMDB_lookup_result_s result =
        lookup_string_ok(mmdb, ip, filename, mode_desc);

    MMDB_entry_data_s entry_data;
    int status =
        MMDB_get_value(&result.entry, &entry_data,
                       "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
                       "11", "12", "13", "14", "15", "16", "17", "18", "19",
                       "20", "21", "22", "23", "24", "25", "26", "27", "28",
                       "29", "30", "31", "32", "33", "34", "35", "36", "37",
                       "38", "39", "40", "41", "42", "43", "44", "45", "46",
                       "47", "48", "49", "50", "51", "52", "53", "54", "55",
                       "56", "57", "58", "59", "60", "61", "62", "63", "64",
                       "65", "66", "67", "68", "69", "70", "71", "72", "73",
                       "74", "75", "76", "77", "78", "79", "80", "81", "82",
                       "83", "84", "85", "86", "87", "88", "89", "90", "91",
                       "92", "93", "94", "95", "96", "97", "98", "99", "100",
                       "101", "102", "103", "104", "105", "106", "107", "108",
                       "109", "110", "111", "112", "113", "114", "115", "116",
                       "117", "118", "119", "120", "121", "122", "123", "124",
                       "125", "126", "127", "128", "129", "130", "131", "132",
                       "133", "134", "135", "136", "137", "138", "139", "140",
                       "141", "142", "143", "144", "145", "146", "147", "148",
                       "149", "150", "151", "152", "153", "154", "155", "156",
                       "157", "158", "159", "160", "161", "162", "163", "164",
                       "165", "166", "167", "168", "169", "170", "171", "172",
                       "173", "174", "175", "176", "177", "178", "179", "180",
                       "181", "182", "183", "184", "185", "186", "187", "188",
                       "189", "190", "191", "192", "193", "194", "195", "196",
                       "197", "198", "199", "200", "201", "202", "203", "204",
                       "205", "206", "207", "208", "209", "210", "211", "212",
                       "213", "214", "215", "216", "217", "218", "219", "220",
                       "221", "222", "223", "224", "225", "226", "227", "228",
                       "229", "230", "231", "232", "233", "234", "235", "236",
                       "237", "238", "239", "240", "241", "242", "243", "244",
                       "245", "246", "247", "248", "249", "250", "251", "252",
                       "253", "254", "255", "256", "257", "258", "259", "260",
                       "261", "262", "263", "264", "265", "266", "267", "268",
                       "269", "270", "271", "272", "273", "274", "275", "276",
                       "277", "278", "279", "280", "281", "282", "283", "284",
                       "285", "286", "287", "288", "289", "290", "291", "292",
                       "293", "294", "295", "296", "297", "298", "299", "300",
                       "301", "302", "303", "304", "305", "306", "307", "308",
                       "309", "310", "311", "312", "313", "314", "315", "316",
                       "317", "318", "319", "320", "321", "322", "323", "324",
                       "325", "326", "327", "328", "329", "330", "331", "332",
                       "333", "334", "335", "336", "337", "338", "339", "340",
                       "341", "342", "343", "344", "345", "346", "347", "348",
                       "349", "350", "351", "352", "353", "354", "355", "356",
                       "357", "358", "359", "360", "361", "362", "363", "364",
                       "365", "366", "367", "368", "369", "370", "371", "372",
                       "373", "374", "375", "376", "377", "378", "379", "380",
                       "381", "382", "383", "384", "385", "386", "387", "388",
                       "389", "390", "391", "392", "393", "394", "395", "396",
                       "397", "398", "399", "400", "401", "402", "403", "404",
                       "405", "406", "407", "408", "409", "410", "411", "412",
                       "413", "414", "415", "416", "417", "418", "419", "420",
                       "421", "422", "423", "424", "425", "426", "427", "428",
                       "429", "430", "431", "432", "433", "434", "435", "436",
                       "437", "438", "439", "440", "441", "442", "443", "444",
                       "445", "446", "447", "448", "449", "450", "451", "452",
                       "453", "454", "455", "456", "457", "458", "459", "460",
                       "461", "462", "463", "464", "465", "466", "467", "468",
                       "469", "470", "471", "472", "473", "474", "475", "476",
                       "477", "478", "479", "480", "481", "482", "483", "484",
                       "485", "486", "487", "488", "489", "490", "491", "492",
                       "493", "494", "495", "496", "497", "498", "499", "500",
                       NULL);
    cmp_ok(status, "==", MMDB_LOOKUP_PATH_IS_TOO_LONG,
           "status for MMDB_vget_value with 501 elements in path");
}

void run_tests(int mode, const char *mode_desc)
{
    test_simple_structure(mode, mode_desc);
    test_nested_structure(mode, mode_desc);
    test_path_too_long(mode, mode_desc);
}

int main(void)
{
    plan(NO_PLAN);
    for_all_modes(&run_tests);
    done_testing();
}
