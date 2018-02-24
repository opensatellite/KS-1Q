new_project \
         -name {test_4463andcpuopration} \
         -location {C:\Users\Administrator\Desktop\10.14_v04workplace\v04workplace\designer\test_4463andcpuopration\test_4463andcpuopration_fp} \
         -mode {chain} \
         -connect_programmers {FALSE}
add_actel_device \
         -device {M2S010S} \
         -name {M2S010S}
enable_device \
         -name {M2S010S} \
         -enable {TRUE}
save_project
close_project
