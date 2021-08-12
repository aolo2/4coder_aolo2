CUSTOM_UI_COMMAND_SIG(aolo2_sync_debugger)
CUSTOM_DOC("Sync with debugger")
{
    Scratch_Block scratch(app);
    
    View_ID current_view = get_active_view(app, Access_ReadWriteVisible);
    Buffer_ID current_buffer = view_get_buffer(app, current_view, Access_ReadWriteVisible);
    i64 current_pos = view_get_cursor_pos(app, current_view);
    
    i64 current_line_number = get_line_number_from_pos(app, current_buffer, current_pos);
    String_Const_u8 file_name = push_buffer_file_name(app, scratch, current_buffer);
    
    String_Const_u8 cmd_file = push_u8_stringf(scratch, "echo f %.*s > $HOME/.config/gf2_control.dat", string_expand(file_name));
    String_Const_u8 cmd_line = push_u8_stringf(scratch, "echo l %d > $HOME/.config/gf2_control.dat", current_line_number);
    String_Const_u8 hot_directory = SCu8("/home/aolo2");
    String_Const_u8 out_buffer = SCu8("*scratch*");
    
    View_ID view = get_active_view(app, Access_Always);
    Buffer_Identifier id = buffer_identifier(out_buffer);
    
    exec_system_command(app, view, id, hot_directory, cmd_file, 0);
    exec_system_command(app, view, id, hot_directory, cmd_line, 0);
}