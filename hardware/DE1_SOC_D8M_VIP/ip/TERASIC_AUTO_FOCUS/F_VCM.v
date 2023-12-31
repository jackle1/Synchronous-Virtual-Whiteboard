module F_VCM  ( 
input RESET_n ,
input CLK  , 
input VS,
input [ 7:0] SCAL,
input [ 7:0] SCAL_F,
output [10:0]STEP,
input  [9:0] STEP_UP , 
output reg V_C ,
output reg VCM_END  ,
output reg GO_F 


) ; 
reg [10:0]STEP_i;
reg [10:0]STEP_f;

//parameter SCAL   = 8'd10 ; 
//parameter SCAL_F = 8'd1 ; 

//---step out 
assign STEP = ( !V_C )? STEP_i  :STEP_f ; 

//---------------------------------------initial setting
reg [9:0] STP_I ; 
always @( negedge RESET_n  or posedge CLK )  
 begin 
 if (!RESET_n )  begin 
     V_C <=0 ; 
	  STEP_i <= 0; 
 end 
  else  begin 
     if(VS)begin
		if    (STEP_i  > 11'h3f0 )
		       V_C <=1 ; 
	   else   STEP_i <= STEP_i + SCAL ;  //10
	  end
    end 
 end 

 
//----------------------------------------fine-adjustment
always @( negedge V_C  or posedge CLK )  
 begin 
 if (!V_C )  begin 
	  STEP_f  <= STEP_UP- SCAL/2; 
	  VCM_END <= 0; 	  
	  GO_F  <=0; 
 end 
  else  begin 
      if(VS)begin
	
			GO_F  <=1; 
			if    (STEP_f  >  STEP_UP + SCAL/2   )
					 VCM_END <=1 ; 
			else   STEP_f <= STEP_f + SCAL_F;  //1
		
		end
		
    end 
 end 

endmodule 
	 