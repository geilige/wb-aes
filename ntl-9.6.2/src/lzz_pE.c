

#include <NTL/lzz_pE.h>

#include <NTL/new.h>

NTL_START_IMPL

zz_pEInfoT::zz_pEInfoT(const zz_pX& NewP)
{
   build(p, NewP);

   _card_base = zz_p::modulus();
   _card_exp = deg(NewP);
}

const ZZ& zz_pE::cardinality()
{
   if (!zz_pEInfo) LogicError("zz_pE::cardinality: undefined modulus");


   do { // NOTE: thread safe lazy init
      Lazy<ZZ>::Builder builder(zz_pEInfo->_card);
      if (!builder()) break;
      UniquePtr<ZZ> p;
      p.make();
      power(*p, zz_pEInfo->_card_base, zz_pEInfo->_card_exp);
      builder.move(p);
   } while (0);

   return *zz_pEInfo->_card;
}





NTL_THREAD_LOCAL SmartPtr<zz_pEInfoT> zz_pEInfo = 0; 


void zz_pE::init(const zz_pX& p)
{
   zz_pEContext c(p);
   c.restore();
}


void zz_pEContext::save()
{
   ptr = zz_pEInfo;
}

void zz_pEContext::restore() const
{
   zz_pEInfo = ptr;
}


zz_pEBak::~zz_pEBak()
{
   if (MustRestore) c.restore();
}

void zz_pEBak::save()
{
   c.save();
   MustRestore = true;
}


void zz_pEBak::restore()
{
   c.restore();
   MustRestore = false;
}



const zz_pE& zz_pE::zero()
{
   NTL_THREAD_LOCAL static zz_pE z(INIT_NO_ALLOC);
   return z;
}




istream& operator>>(istream& s, zz_pE& x)
{
   zz_pX y;

   NTL_INPUT_CHECK_RET(s, s >> y);
   conv(x, y);

   return s;
}

void div(zz_pE& x, const zz_pE& a, const zz_pE& b)
{
   zz_pE t;

   inv(t, b);
   mul(x, a, t);
}

void div(zz_pE& x, const zz_pE& a, long b)
{
   NTL_zz_pRegister(B);
   B = b;
   inv(B, B);
   mul(x, a, B);
}

void div(zz_pE& x, const zz_pE& a, const zz_p& b)
{
   NTL_zz_pRegister(B);
   B = b;
   inv(B, B);
   mul(x, a, B);
}

void div(zz_pE& x, long a, const zz_pE& b)
{
   zz_pE t;
   inv(t, b);
   mul(x, a, t);
}

void div(zz_pE& x, const zz_p& a, const zz_pE& b)
{
   zz_pE t;
   inv(t, b);
   mul(x, a, t);
}



void inv(zz_pE& x, const zz_pE& a)
{
   InvMod(x._zz_pE__rep, a._zz_pE__rep, zz_pE::modulus());
}

NTL_END_IMPL
