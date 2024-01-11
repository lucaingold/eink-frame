#include "../lib/Config/DEV_Config.h"
#include "example.h"
#include "../lib/GUI/GUI_BMPfile.h"

#include <microhttpd.h>

#include <math.h>

#include <stdlib.h>     //exit()
#include <signal.h>     //signal()

#include <unistd.h>

#define PORT 8080

UWORD VCOM = 2510;

IT8951_Dev_Info Dev_Info = {0, 0};
UWORD Panel_Width;
UWORD Panel_Height;
UDOUBLE Init_Target_Memory_Addr;
int epd_mode = 0;	//0: no rotate, no mirror
					//1: no rotate, horizontal mirror, for 10.3inch
					//2: no totate, horizontal mirror, for 5.17inch
					//3: no rotate, no mirror, isColor, for 6inch color
					
void  Handler(int signo){
    Debug("\r\nHandler:exit\r\n");
    if(Refresh_Frame_Buf != NULL){
        free(Refresh_Frame_Buf);
        Debug("free Refresh_Frame_Buf\r\n");
        Refresh_Frame_Buf = NULL;
    }
    if(Panel_Frame_Buf != NULL){
        free(Panel_Frame_Buf);
        Debug("free Panel_Frame_Buf\r\n");
        Panel_Frame_Buf = NULL;
    }
    if(Panel_Area_Frame_Buf != NULL){
        free(Panel_Area_Frame_Buf);
        Debug("free Panel_Area_Frame_Buf\r\n");
        Panel_Area_Frame_Buf = NULL;
    }
    if(bmp_src_buf != NULL){
        free(bmp_src_buf);
        Debug("free bmp_src_buf\r\n");
        bmp_src_buf = NULL;
    }
    if(bmp_dst_buf != NULL){
        free(bmp_dst_buf);
        Debug("free bmp_dst_buf\r\n");
        bmp_dst_buf = NULL;
    }
	if(Dev_Info.Panel_W != 0){
		Debug("Going to sleep\r\n");
		EPD_IT8951_Sleep();
	}
    DEV_Module_Exit();
    exit(0);
}


// Declaration of the HTTP request handler function
int answer_to_connection(void *cls, struct MHD_Connection *connection,
                          const char *url, const char *method,
                          const char *version, const char *upload_data,
                          size_t *upload_data_size, void **con_cls);


void start_http_server();

void  initScreen(){
    Dev_Info = EPD_IT8951_Init(VCOM);

    //get some important info from Dev_Info structure
    Panel_Width = Dev_Info.Panel_W;
    Panel_Height = Dev_Info.Panel_H;
    Init_Target_Memory_Addr = Dev_Info.Memory_Addr_L | (Dev_Info.Memory_Addr_H << 16);
    char* LUT_Version = (char*)Dev_Info.LUT_Version;
    if( strcmp(LUT_Version, "M641") == 0 ){
        //6inch e-Paper HAT(800,600), 6inch HD e-Paper HAT(1448,1072), 6inch HD touch e-Paper HAT(1448,1072)
        A2_Mode = 4;
        Four_Byte_Align = true;
    }else if( strcmp(LUT_Version, "M841_TFAB512") == 0 ){
        //Another firmware version for 6inch HD e-Paper HAT(1448,1072), 6inch HD touch e-Paper HAT(1448,1072)
        A2_Mode = 6;
        Four_Byte_Align = true;
    }else if( strcmp(LUT_Version, "M841") == 0 ){
        //9.7inch e-Paper HAT(1200,825)
        A2_Mode = 6;
    }else if( strcmp(LUT_Version, "M841_TFA2812") == 0 ){
        //7.8inch e-Paper HAT(1872,1404)
        A2_Mode = 6;
    }else if( strcmp(LUT_Version, "M841_TFA5210") == 0 ){
        //10.3inch e-Paper HAT(1872,1404)
        A2_Mode = 6;
    }else{
        //default set to 6 as A2 Mode
        A2_Mode = 6;
    }
    Debug("A2 Mode:%d\r\n", A2_Mode);

	EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);

}
int main(int argc, char *argv[])
{
    //Exception handling:ctrl + c
    signal(SIGINT, Handler);

    if (argc < 2){
        Debug("Please input VCOM value on FPC cable!\r\n");
        Debug("Example: sudo ./epd -2.51\r\n");
        exit(1);
    }
	if (argc != 3){
		Debug("Please input e-Paper display mode!\r\n");
		Debug("Example: sudo ./epd -2.27 0 or sudo ./epd -2.51 1\r\n");
		Debug("Now, 10.3 inch glass panle is mode1, else is mode0\r\n");
		Debug("If you don't know what to type in just type 0 \r\n");
		exit(1);
    }

    //Init the BCM2835 Device
    if(DEV_Module_Init()!=0){
        return -1;
    }

    double temp;
    sscanf(argv[1],"%lf",&temp);
    VCOM = (UWORD)(fabs(temp)*1000);
    Debug("VCOM value:%d\r\n", VCOM);
	sscanf(argv[2],"%d",&epd_mode);
    Debug("Display mode:%d\r\n", epd_mode);
    
    start_http_server();  // Start the HTTP server

    // Exception handling: ctrl + c
    signal(SIGINT, Handler);

// // DEMO START

//     //Show 16 grayscale
//     Display_ColorPalette_Example(Panel_Width, Panel_Height, Init_Target_Memory_Addr);
// 	EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, GC16_Mode);

//     //Show a bmp file
//     //1bp use A2 mode by default, before used it, refresh the screen with WHITE
//     Display_BMP_Example(Panel_Width, Panel_Height, Init_Target_Memory_Addr, BitsPerPixel_1);
//     Display_BMP_Example(Panel_Width, Panel_Height, Init_Target_Memory_Addr, BitsPerPixel_2);
//     Display_BMP_Example(Panel_Width, Panel_Height, Init_Target_Memory_Addr, BitsPerPixel_4);
//     EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, GC16_Mode);
    
//     //Show A2 mode refresh effect
//     EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, A2_Mode);
//     Dynamic_Refresh_Example(Dev_Info,Init_Target_Memory_Addr);
//     EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, A2_Mode);
//     EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, GC16_Mode);


// // DEMO END

    // //We recommended refresh the panel to white color before storing in the warehouse.
    // EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);

    //EPD_IT8951_Standby();

    
    // EPD_IT8951_Sleep();

    // //In case RPI is transmitting image in no hold mode, which requires at most 10s
    // DEV_Delay_ms(5000);

    // DEV_Module_Exit();
    return 0;
}

// Function to handle POST requests with paths in the body
int answer_to_connection(void *cls, struct MHD_Connection *connection,
                         const char *url, const char *method,
                         const char *version, const char *upload_data,
                         size_t *upload_data_size, void **con_cls) {
    static int aptr;
    static char *post_data = NULL;

    if (*con_cls == NULL) {
        printf("New connection: %s %s\n", method, url);
        *con_cls = (void *)1;
        aptr = 0;
        return MHD_YES;
    }

    if (*upload_data_size > 0) {
        // Accumulate the incoming data
        post_data = realloc(post_data, aptr + *upload_data_size + 1);
        if (post_data == NULL) {
            // Handle memory allocation failure
            return MHD_NO;
        }
        memcpy(post_data + aptr, upload_data, *upload_data_size);
        aptr += *upload_data_size;
        post_data[aptr] = '\0';
        *upload_data_size = 0;  // Reset for the next chunk of data
        return MHD_YES;
    }

    // Process the complete string data in post_data
    // You can perform any custom logic based on the received string

    // For demonstration purposes, let's print the received string
    printf("Received POST data: %s\n", post_data);

    // Extract the path from the post_data and call Display_BMP_Example
    if (strncmp(post_data, "path=", 5) == 0) {
        const char *path = post_data + 5;  // Skip "path="
        printf("Path is: %s\n", path);
        initScreen();
        EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, GC16_Mode);
        Display_BMP_WITH_PATH(Panel_Width, Panel_Height, Init_Target_Memory_Addr, BitsPerPixel_4, path);
    } else {
        // Handle invalid or missing path in the post_data
        EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, GC16_Mode);
    }

    EPD_IT8951_Sleep();
    printf("Display is in sleep mode\n");

    // Respond to the client
    const char *response = "<html><body>POST request with path received!</body></html>";
    struct MHD_Response *resp = MHD_create_response_from_buffer(strlen(response), (void *)response, MHD_RESPMEM_PERSISTENT);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, resp);
    MHD_destroy_response(resp);

    // Clean up and reset
    free(post_data);
    post_data = NULL;
    aptr = 0;



    return ret;
}

// Function to start the HTTP server
void start_http_server() {
    struct MHD_Daemon *daemon;

    // Initialize the HTTP server daemon
    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                              &answer_to_connection, NULL, MHD_OPTION_END);
    if (daemon == NULL) {
        fprintf(stderr, "Failed to start the HTTP server\n");
        exit(EXIT_FAILURE);
    }

    printf("HTTP server started on port %d\n", PORT);

    // Wait for a key press to stop the server
    printf("Press any key to stop the server...\n");
    (void)getc(stdin);

    // Stop the HTTP server daemon
    MHD_stop_daemon(daemon);
}