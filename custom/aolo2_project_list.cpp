CUSTOM_UI_COMMAND_SIG(aolo2_project_list)
CUSTOM_DOC("Project switch")
{
    Scratch_Block scratch(app);
    String_Const_u8 name = { (u8 *) "/opt/4coder/projects", 20 };
    
    
    if (file_exists_and_is_file(app, name)) {
        FILE *file = open_file(scratch, name);
        String_Const_u8 data = dump_file_handle(scratch, file);
        fclose(file);
        char *query = "Project:";
        
        Lister_Block lister(app, scratch);
        lister_set_query(lister, query);
        lister_set_default_handlers(lister);
        
        u8 *path = data.str;
        int i = 0;
        bool newline = true;
        
        while (i < data.size) {
            int j = i;
            while (j < data.size && data.str[j] != ' ') {
                ++j;
            }
            
            if (j - i > 0) {
                String_Const_u8 *fullpath = push_array(scratch, String_Const_u8, 1);
                fullpath->str = data.str + i;
                fullpath->size = j - i;
                
                // skip space between fullpath and project name
                ++j;
                
                String_Const_u8 project_name;
                int k = j;
                
                while (k < data.size && data.str[k] != '\n') {
                    ++k;
                }
                
                project_name.str = data.str + j;
                project_name.size = k - j;
                
                lister_add_item(lister, project_name, *fullpath, (void *) fullpath, 0);
                
                i = k + 1;
            } else {
                break;
            }
        }
        
        Lister_Result l_result = run_lister(app, lister);
        String_Const_u8 result = {};
        if (!l_result.canceled && l_result.user_data != 0){
            block_copy_struct(&result, (String_Const_u8*)l_result.user_data);
        }
        
        if (result.size != 0) {
            String_Const_u8 project_root_directory = result;
            int j = result.size - 1;
            while (j >= 0 && result.str[j] != '/') {
                --j;
            }
            
            if (j > 0) {
                project_root_directory.size = j + 1;
                set_hot_directory(app, project_root_directory);
                load_project(app);
            }
        }
    }
}