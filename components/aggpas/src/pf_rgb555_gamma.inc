//
// AggPas 2.4 RM3 pixel format definition file
//
{ make_pix_555_gamma }
function make_pix_555_gamma(r ,g ,b : unsigned ) : int16u;
begin
 result:=int16u(
  ((r and $F8 ) shl 7 ) or
  ((g and $F8 ) shl 2 ) or
  (b shr 3 ) or
  $8000 );

end;

{ make_color_555_gamma }
procedure make_color_555_gamma(var color : aggclr; p : int16u_ptr );
begin
 color.ConstrInt(
  (p^ shr 7 ) and $F8 ,
  (p^ shr 2 ) and $F8 ,
  (p^ shl 3 ) and $F8 )

end;

{ blend_pix_555_gamma }
procedure blend_pix_555_gamma(gamma : gamma_ptr; p : int16u_ptr; cr ,cg ,cb ,alpha ,cover : unsigned );
var
 rgb : int16u;

 r ,g ,b : int;

begin
 rgb:=p^;

 r:=gamma.dir((rgb shr 7 ) and $F8 );
 g:=gamma.dir((rgb shr 2 ) and $F8 );
 b:=gamma.dir((rgb shl 3 ) and $F8 );

 p^:=int16u(
  ((gamma.inv(((gamma.dir(cr ) - r ) * alpha + (r shl 8 ) ) shr 8 ) shl 7 ) and $7C00 ) or
  ((gamma.inv(((gamma.dir(cg ) - g ) * alpha + (g shl 8 ) ) shr 8 ) shl 2 ) and $03E0 ) or
   (gamma.inv(((gamma.dir(cb ) - b ) * alpha + (b shl 8 ) ) shr 8 ) shr 3 ) or $8000 );

end;

{ copy_or_blend_pix_555_gamma }
procedure copy_or_blend_pix_555_gamma(gamma : gamma_ptr; p : int16u_ptr; c : aggclr_ptr; cover : unsigned );
var
 alpha : unsigned;

begin
 if c.a <> 0 then
  begin
   alpha:=(c.a * (cover + 1 ) ) shr 8;

   if alpha = base_mask then
    p^:=make_pix_555_gamma(c.r ,c.g ,c.b )
   else
    blend_pix_555_gamma(gamma ,p ,c.r ,c.g ,c.b ,alpha ,cover );

  end;

end;

{ rgb555_gamma_copy_pixel }
procedure rgb555_gamma_copy_pixel(this : pixel_formats_ptr; x ,y : int; c : aggclr_ptr );
begin
 int16u_ptr(ptrcomp(this.m_rbuf.row(y ) ) + x * sizeof(int16u ) )^:=make_pix_555_gamma(c.r ,c.g ,c.b );

end;

{ rgb555_gamma_blend_pixel }
procedure rgb555_gamma_blend_pixel(this : pixel_formats_ptr; x ,y : int; c : aggclr_ptr; cover : int8u );
begin
 copy_or_blend_pix_555_gamma(this.m_gamma ,int16u_ptr(ptrcomp(this.m_rbuf.row(y ) ) + x * sizeof(int16u ) ) ,c ,cover );

end;

{ rgb555_gamma_pixel }
function rgb555_gamma_pixel(this : pixel_formats_ptr; x ,y : int ) : aggclr;
begin
 make_color_555_gamma(
  result ,
  int16u_ptr(ptrcomp(this.m_rbuf.row(y ) ) + x * sizeof(int16u ) ) );

end;

{ rgb555_gamma_copy_hline }
procedure rgb555_gamma_copy_hline(this : pixel_formats_ptr; x ,y : int; len : unsigned; c : aggclr_ptr );
var
 p : int16u_ptr;
 v : int16u;

begin
 p:=int16u_ptr        (ptrcomp(this.m_rbuf.row(y ) ) + x * sizeof(int16u ) );
 v:=make_pix_555_gamma(c.r ,c.g ,c.b );

 repeat
  p^:=v;

  inc(ptrcomp(p ) ,sizeof(int16u ) );
  dec(len );

 until len = 0;

end;

{ rgb555_gamma_copy_vline }
procedure rgb555_gamma_copy_vline(this : pixel_formats_ptr; x ,y : int; len : unsigned; c : aggclr_ptr );
var
 p : int16u_ptr;
 v : int16u;

begin
 p:=int16u_ptr        (ptrcomp(this.m_rbuf.row(y ) ) + x * sizeof(int16u ) );
 v:=make_pix_555_gamma(c.r ,c.g ,c.b );

 repeat
  p^:=v;
  p :=int16u_ptr(this.m_rbuf.next_row(int8u_ptr(p ) ) );

  dec(len );

 until len = 0;

end;

{ rgb555_gamma_blend_hline }
procedure rgb555_gamma_blend_hline(this : pixel_formats_ptr; x ,y : int; len : unsigned; c : aggclr_ptr; cover : int8u );
var
 p : int16u_ptr;
 v : int16u;

 alpha : unsigned;

begin
 if c.a <> 0 then
  begin
   p:=int16u_ptr(ptrcomp(this.m_rbuf.row(y ) ) + x * sizeof(int16u ) );

   alpha:=(c.a * (cover + 1 ) ) shr 8;

   if alpha = base_mask then
    begin
     v:=make_pix_555_gamma(c.r ,c.g ,c.b );

     repeat
      p^:=v;

      inc(ptrcomp(p ) ,sizeof(int16u ) );
      dec(len );

     until len = 0;

    end
   else
    repeat
     blend_pix_555_gamma(this.m_gamma ,p ,c.r ,c.g ,c.b ,alpha ,cover );

     inc(ptrcomp(p ) ,sizeof(int16u ) );
     dec(len );

    until len = 0;

  end;

end;

{ rgb555_gamma_blend_vline }
procedure rgb555_gamma_blend_vline(this : pixel_formats_ptr; x ,y : int; len : unsigned; c : aggclr_ptr; cover : int8u );
var
 p : int16u_ptr;
 v : int16u;

 alpha : unsigned;

begin
 if c.a <> 0 then
  begin
   p:=int16u_ptr(ptrcomp(this.m_rbuf.row(y ) ) + x * sizeof(int16u ) );

   alpha:=(c.a * (cover + 1 ) ) shr 8;

   if alpha = base_mask then
    begin
     v:=make_pix_555_gamma(c.r ,c.g ,c.b );

     repeat
      p^:=v;
      p :=int16u_ptr(this.m_rbuf.next_row(int8u_ptr(p ) ) );

      dec(len );

     until len = 0;

    end
   else
    repeat
     blend_pix_555_gamma(this.m_gamma ,p ,c.r ,c.g ,c.b ,alpha ,cover );

     p:=int16u_ptr(this.m_rbuf.next_row(int8u_ptr(p ) ) );

     dec(len );

    until len = 0;

  end;

end;

{ rgb555_gamma_blend_solid_hspan }
procedure rgb555_gamma_blend_solid_hspan(this : pixel_formats_ptr; x ,y : int; len : unsigned; c : aggclr_ptr; covers : int8u_ptr );
var
 p : int16u_ptr;

begin
 p:=int16u_ptr(ptrcomp(this.m_rbuf.row(y ) ) + x * sizeof(int16u ) );

 repeat
  copy_or_blend_pix_555_gamma(this.m_gamma ,p ,c ,covers^ );

  inc(ptrcomp(covers ) );
  inc(ptrcomp(p ) ,sizeof(int16u ) );
  dec(len );

 until len = 0;

end;

{ rgb555_gamma_blend_solid_vspan }
procedure rgb555_gamma_blend_solid_vspan(this : pixel_formats_ptr; x ,y : int; len : unsigned; c : aggclr_ptr; covers : int8u_ptr );
var
 p : int16u_ptr;

begin
 p:=int16u_ptr(ptrcomp(this.m_rbuf.row(y ) ) + x * sizeof(int16u ) );

 repeat
  copy_or_blend_pix_555_gamma(this.m_gamma ,p ,c ,covers^ );

  inc(ptrcomp(covers ) );

  p:=int16u_ptr(this.m_rbuf.next_row(int8u_ptr(p ) ) );

  dec(len );

 until len = 0;

end;

{ rgb555_gamma_blend_color_hspan }
procedure rgb555_gamma_blend_color_hspan(this : pixel_formats_ptr; x ,y : int; len : unsigned; colors : aggclr_ptr; covers : int8u_ptr; cover : int8u );
var
 p : int16u_ptr;

begin
 p:=int16u_ptr(ptrcomp(this.m_rbuf.row(y ) ) + x * sizeof(int16u ) );

 repeat
  if covers <> NIL then
   begin
    copy_or_blend_pix_555_gamma(this.m_gamma ,p ,colors ,covers^ );

    inc(ptrcomp(covers ) ,sizeof(int8u ) );

   end
  else
   copy_or_blend_pix_555_gamma(this.m_gamma ,p ,colors ,cover );

  inc(ptrcomp(p ) ,sizeof(int16u ) );
  inc(ptrcomp(colors ) ,sizeof(aggclr ) );
  dec(len );

 until len = 0;

end;

{ rgb555_gamma_blend_color_vspan }
procedure rgb555_gamma_blend_color_vspan(this : pixel_formats_ptr; x ,y : int; len : unsigned; colors : aggclr_ptr; covers : int8u_ptr; cover : int8u );
var
 p : int16u_ptr;

begin
 p:=int16u_ptr(ptrcomp(this.m_rbuf.row(y ) ) + x * sizeof(int16u ) );

 repeat
  if covers <> NIL then
   begin
    copy_or_blend_pix_555_gamma(this.m_gamma ,p ,colors ,covers^ );

    inc(ptrcomp(covers ) ,sizeof(int8u ) );

   end
  else
   copy_or_blend_pix_555_gamma(this.m_gamma ,p ,colors ,cover );

  p:=int16u_ptr(this.m_rbuf.next_row(int8u_ptr(p ) ) );

  inc(ptrcomp(colors ) ,sizeof(aggclr ) );
  dec(len );

 until len = 0;

end;

{ rgb555_gamma_copy_from }
procedure rgb555_gamma_copy_from(this : pixel_formats_ptr; from : rendering_buffer_ptr; xdst ,ydst ,xsrc ,ysrc : int; len : unsigned );
begin
 move(
  int16u_ptr(ptrcomp(from.row(ysrc ) ) + xsrc * sizeof(int16u ) )^ ,
  int16u_ptr(ptrcomp(this.m_rbuf.row(ydst ) ) + xdst * sizeof(int16 ) )^ ,
  len * sizeof(int16u ) );

end;

{ rgb555_gamma_blend_from }
procedure rgb555_gamma_blend_from(this : pixel_formats_ptr; from : pixel_formats_ptr; psrc_ : int8u_ptr; xdst ,ydst ,xsrc ,ysrc : int; len : unsigned; cover : int8u );
var
 psrc : int8u_ptr;
 pdst : int16u_ptr;

 alpha : unsigned;

begin
 psrc:=psrc_;
 pdst:=int16u_ptr(ptrcomp(this.m_rbuf.row(ydst ) ) + xdst * sizeof(int16u ) );

 repeat
  alpha:=int8u_ptr(ptrcomp(psrc ) + from.m_order.A )^;

  if alpha <> 0 then
   if (alpha = base_mask ) and
      (cover = 255 ) then
    pdst^:=make_pix_555_gamma(
     int8u_ptr(ptrcomp(psrc ) + from.m_order.R )^ ,
     int8u_ptr(ptrcomp(psrc ) + from.m_order.G )^ ,
     int8u_ptr(ptrcomp(psrc ) + from.m_order.B )^ )
   else
    blend_pix_555_gamma(
     this.m_gamma ,
     pdst ,
     int8u_ptr(ptrcomp(psrc ) + from.m_order.R )^ ,
     int8u_ptr(ptrcomp(psrc ) + from.m_order.G )^ ,
     int8u_ptr(ptrcomp(psrc ) + from.m_order.B )^ ,
     alpha ,cover );

  inc(ptrcomp(psrc ) ,4 );
  inc(ptrcomp(pdst ) ,sizeof(int16u ) );
  dec(len );

 until len = 0;

end;

{ rgb555_gamma_copy_color_hspan }
procedure rgb555_gamma_copy_color_hspan(this : pixel_formats_ptr; x ,y : int; len : unsigned; colors : aggclr_ptr );
var
 p : int16u_ptr;

begin
 p:=int16u_ptr(ptrcomp(this.m_rbuf.row(y ) ) + x * sizeof(int16u ) );

 repeat
  p^:=make_pix_555_gamma(colors.r ,colors.g ,colors.b );

  inc(ptrcomp(p ) ,sizeof(int16u ) );
  inc(ptrcomp(colors ) ,sizeof(aggclr ) );
  dec(len );

 until len = 0;

end;

{ rgb555_gamma_copy_color_vspan }
procedure rgb555_gamma_copy_color_vspan(this : pixel_formats_ptr; x ,y : int; len : unsigned; colors : aggclr_ptr );
var
 p : int16u_ptr;

begin
 p:=int16u_ptr(ptrcomp(this.m_rbuf.row(y ) ) + x * sizeof(int16u ) );

 repeat
  p^:=make_pix_555_gamma(colors.r ,colors.g ,colors.b );
  p :=int16u_ptr(this.m_rbuf.next_row(int8u_ptr(p ) ) );

  inc(ptrcomp(colors ) ,sizeof(aggclr ) );
  dec(len );

 until len = 0;

end;

{ rgb555_gamma_blend_from_color }
procedure rgb555_gamma_blend_from_color(this : pixel_formats_ptr; from : pixel_formats_ptr; color : aggclr_ptr; xdst ,ydst ,xsrc ,ysrc : int; len : unsigned; cover : int8u );
var
 ppsz : unsigned;

 psrc : int8u_ptr;
 pdst : int16u_ptr;

begin
 ppsz:=from._pix_width;
 psrc:=from.row_ptr(ysrc );

 if psrc <> NIL then
  begin
   pdst:=int16u_ptr(ptrcomp(this.m_rbuf.row_xy(xdst ,ydst ,len ) ) + xdst * sizeof(int16u ) );

   repeat
    blend_pix_555_gamma(
     this.m_gamma ,
     pdst ,color.r ,color.g ,color.b ,color.a ,shr_int32(psrc^ * cover + base_mask ,base_shift ) );

    inc(ptrcomp(psrc ) ,ppsz );
    inc(ptrcomp(pdst ) ,sizeof(int16u ) );
    dec(len );

   until len = 0;

  end;

end;

{ rgb555_gamma_blend_from_lut }
procedure rgb555_gamma_blend_from_lut(this : pixel_formats_ptr; from : pixel_formats_ptr; color_lut : aggclr_ptr; xdst ,ydst ,xsrc ,ysrc : int; len : unsigned; cover : int8u );
var
 ppsz : unsigned;

 psrc : int8u_ptr;
 pdst : int16u_ptr;

 color : aggclr_ptr;

begin
 ppsz:=from._pix_width;
 psrc:=from.row_ptr(ysrc );

 if psrc <> NIL then
  begin
   pdst:=int16u_ptr(ptrcomp(this.m_rbuf.row_xy(xdst ,ydst ,len ) ) + xdst * sizeof(int16u ) );

   repeat
    color:=aggclr_ptr(ptrcomp(color_lut ) + psrc^ * sizeof(aggclr ) );

    blend_pix_555_gamma(
     this.m_gamma ,
     pdst ,color.r ,color.g ,color.b ,color.a ,shr_int32(psrc^ * cover + base_mask ,base_shift ) );

    inc(ptrcomp(psrc ) ,ppsz );
    inc(ptrcomp(pdst ) ,sizeof(int16u ) );
    dec(len );

   until len = 0;

  end;

end;

