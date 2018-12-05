// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
/////////////////////////////////////////////////////////
// includes
/////////////////////////////////////////////////////////
#include "mlBilat.h"
#include "math_fns.h"
#include "mlShared.h"
#include "bar.h"
#include "utils.h"
#include "bench.h"

/////////////////////////////////////////////////////////
// shared globals
/////////////////////////////////////////////////////////

  __attribute__ ((section(".heapsram"))) static const float fv1[2] = { 1.0F, 0.05F };

  __attribute__ ((section(".heapsram"))) static const float fv2[900] = { 0.251995474F, 0.180555314F, 0.247393414F,
    0.129591689F, 0.182139516F, 0.204966694F, 0.166609243F, 0.251765579F,
    0.167272523F, 0.087269783F, 0.247072577F, 0.0747126415F, 0.136028796F,
    0.144114688F, 0.175539568F, 0.171606526F, 0.235553622F, 0.208221748F,
    0.160873175F, 0.218678802F, 0.176320776F, 0.207384884F, 0.199812979F,
    0.177919164F, 0.297525465F, 0.13060908F, 0.163530573F, 0.169108883F,
    0.220637202F, 0.166322693F, 0.242051542F, 0.131931394F, 0.290760785F,
    0.230232596F, 0.195397466F, 0.126060978F, 0.126673058F, 0.147964016F,
    0.051301755F, 0.162134707F, 0.115817845F, 0.235899046F, 0.26736F,
    0.145611286F, 0.184759423F, 0.195746541F, 0.125687689F, 0.232725173F,
    0.143522784F, 0.060692817F, 0.176619127F, 0.143506199F, 0.148195341F, 0.095F,
    0.186062962F, 0.157429576F, 0.153991222F, 0.136827171F, 0.190262988F,
    0.241306335F, 0.0965846106F, 0.194732219F, 0.158517867F, 0.154253423F,
    0.209028274F, 0.171771467F, 0.155516624F, 0.22461839F, 0.15567112F,
    0.151814908F, 0.269987851F, 0.126270905F, 0.147424936F, 0.229218781F,
    0.251232654F, 0.143310979F, 0.108096205F, 0.222758964F, 0.200190663F,
    0.208756655F, 0.160569906F, 0.242266148F, 0.141663283F, 0.201559782F,
    0.169102952F, 0.139722645F, 0.243071288F, 0.220480844F, 0.252788484F,
    0.20786728F, 0.259233862F, 0.216007262F, 0.161858767F, 0.14779368F,
    0.205926582F, 0.215915486F, 0.227242678F, 0.120753743F, 0.133447379F,
    0.26519537F, 0.20136416F, 0.202304587F, 0.186986312F, 0.228102624F,
    0.227934271F, 0.237540528F, 0.205664724F, 0.175221205F, 0.18037805F,
    0.143677339F, 0.088846609F, 0.255305648F, 0.23534371F, 0.191433221F,
    0.234909192F, 0.339033365F, 0.266935229F, 0.437770396F, 0.310264468F,
    0.319240034F, 0.267065555F, 0.247450113F, 0.200000718F, 0.214334652F,
    0.10988497F, 0.265281856F, 0.342636734F, 0.327100933F, 0.324591F,
    0.257317394F, 0.376876831F, 0.365882635F, 0.4914F, 0.410409838F,
    0.378909081F, 0.0943677723F, 0.170011044F, 0.214956045F, 0.196622223F,
    0.336730301F, 0.387408614F, 0.364551455F, 0.440908194F, 0.467653096F,
    0.393857449F, 0.345268F, 0.335531473F, 0.434917092F, 0.362576127F,
    0.37059167F, 0.178899303F, 0.224103227F, 0.163533837F, 0.371633679F,
    0.42469418F, 0.393084258F, 0.362167954F, 0.419045478F, 0.278215408F,
    0.323530197F, 0.37495935F, 0.326826751F, 0.385186076F, 0.448575854F,
    0.410844475F, 0.20139052F, 0.176221848F, 0.400640666F, 0.473216861F,
    0.380097151F, 0.390363067F, 0.401987314F, 0.38622734F, 0.494037718F,
    0.36801061F, 0.282755F, 0.24779731F, 0.412815899F, 0.40542224F, 0.451552421F,
    0.152417541F, 0.205180824F, 0.423079073F, 0.422351122F, 0.451316327F,
    0.400750875F, 0.486376345F, 0.398215473F, 0.345551521F, 0.357689053F,
    0.270974666F, 0.366916984F, 0.400613338F, 0.416159391F, 0.391042888F,
    0.299478024F, 0.306459665F, 0.358053684F, 0.40454489F, 0.471192658F,
    0.450451076F, 0.310454875F, 0.399162412F, 0.447361439F, 0.430117249F,
    0.435159922F, 0.370557517F, 0.357429028F, 0.424878299F, 0.513916969F,
    0.229407966F, 0.281084418F, 0.335203379F, 0.439121366F, 0.525993347F,
    0.336071432F, 0.3194F, 0.33691293F, 0.41214782F, 0.428852469F, 0.408749342F,
    0.340998322F, 0.256056845F, 0.474103749F, 0.426734686F, 0.243838534F,
    0.222483248F, 0.275075406F, 0.365599871F, 0.437485099F, 0.405481428F,
    0.451708049F, 0.467214853F, 0.297504097F, 0.333882272F, 0.318774313F,
    0.286316335F, 0.354311168F, 0.379280955F, 0.398584098F, 0.196367174F,
    0.320997089F, 0.394379199F, 0.40331158F, 0.424956471F, 0.469432265F,
    0.319415748F, 0.330963F, 0.336292982F, 0.356871456F, 0.330013841F,
    0.338173836F, 0.335912496F, 0.354938418F, 0.345373243F, 0.120181084F,
    0.300371468F, 0.448368222F, 0.500730038F, 0.412108183F, 0.451092154F,
    0.362458348F, 0.379122943F, 0.355226785F, 0.34007442F, 0.260773897F,
    0.314795792F, 0.353530169F, 0.353059679F, 0.484175026F, 0.11045675F,
    0.205807269F, 0.479162723F, 0.478855371F, 0.40792951F, 0.437977105F,
    0.425440758F, 0.410269707F, 0.393613189F, 0.368500233F, 0.350537568F,
    0.27410239F, 0.310682297F, 0.410116583F, 0.422821224F, 0.185129821F,
    0.399205416F, 0.480494916F, 0.51050669F, 0.513724923F, 0.386202812F,
    0.463514686F, 0.339069694F, 0.391619772F, 0.371746808F, 0.284742445F,
    0.268745065F, 0.356823266F, 0.425378352F, 0.423307568F, 0.244274348F,
    0.360140204F, 0.409923613F, 0.479396373F, 0.69415307F, 0.446410298F,
    0.363605827F, 0.291457236F, 0.431699365F, 0.444971919F, 0.394227505F,
    0.31197843F, 0.258339256F, 0.297025651F, 0.363192022F, 0.28437686F,
    0.41517818F, 0.448796F, 0.445603788F, 0.485210568F, 0.446533382F,
    0.451949596F, 0.414069235F, 0.454940259F, 0.382307768F, 0.428123295F,
    0.316117823F, 0.373323053F, 0.376535058F, 0.346542F, 0.329131246F,
    0.453794152F, 0.322620511F, 0.436145931F, 0.443976909F, 0.470132858F,
    0.421054244F, 0.3351143F, 0.434824347F, 0.438887358F, 0.370637149F,
    0.439070284F, 0.342019498F, 0.254405022F, 0.316705436F, 0.293690741F,
    0.466174841F, 0.493646532F, 0.49509576F, 0.421089351F, 0.401275337F,
    0.40816474F, 0.365865767F, 0.43091467F, 0.478856623F, 0.41132009F,
    0.368919313F, 0.359873384F, 0.386979192F, 0.325460941F, 0.249212161F,
    0.422462225F, 0.479779303F, 0.461400062F, 0.455509484F, 0.429589927F,
    0.394942224F, 0.464883476F, 0.448185861F, 0.415702701F, 0.501763403F,
    0.326889724F, 0.306393F, 0.377216071F, 0.334774435F, 0.279678F, 0.418549031F,
    0.474675298F, 0.56631732F, 0.471212268F, 0.487017035F, 0.492384195F,
    0.560794055F, 0.517361939F, 0.473433763F, 0.406620979F, 0.376649708F,
    0.342315882F, 0.366448939F, 0.22853817F, 0.383129954F, 0.409772307F,
    0.489951164F, 0.506441712F, 0.491941184F, 0.420466572F, 0.566392601F,
    0.759846747F, 0.605604231F, 0.466376424F, 0.456078231F, 0.357445419F,
    0.303804666F, 0.265508175F, 0.361018538F, 0.382631749F, 0.492379963F,
    0.448631525F, 0.491233F, 0.588832736F, 0.478667408F, 0.529110372F,
    0.566275299F, 0.467371523F, 0.489801943F, 0.461626589F, 0.230426192F,
    0.260663301F, 0.171124801F, 0.236308306F, 0.340233177F, 0.494375885F,
    0.619182527F, 0.587515414F, 0.49505955F, 0.516821563F, 0.606078F,
    0.687037289F, 0.558927834F, 0.483471155F, 0.310793698F, 0.289903164F,
    0.345055938F, 0.363612443F, 0.267178506F, 0.16007024F, 0.374731123F,
    0.50767225F, 0.502249062F, 0.441452861F, 0.598686218F, 0.601666212F,
    0.617948532F, 0.553800821F, 0.439600378F, 0.290862918F, 0.408202142F,
    0.253521681F, 0.276938379F, 0.301313043F, 0.308357745F, 0.326887697F,
    0.309713244F, 0.563093603F, 0.462125063F, 0.387627184F, 0.556601107F,
    0.728703558F, 0.569051564F, 0.532592177F, 0.470247298F, 0.410726368F,
    0.323355436F, 0.308090329F, 0.261064857F, 0.372308582F, 0.499248087F,
    0.455262959F, 0.459713221F, 0.512686372F, 0.443617672F, 0.461882979F,
    0.695126355F, 0.519824147F, 0.440720797F, 0.493167698F, 0.367092133F,
    0.182542056F, 0.240985617F, 0.252655983F, 0.402777374F, 0.539619207F,
    0.445961297F, 0.605589747F, 0.578427374F, 0.442795813F, 0.484399945F,
    0.683327436F, 0.607570827F, 0.540308714F, 0.40282169F, 0.28485021F,
    0.190590262F, 0.300542891F, 0.189209074F, 0.312187374F, 0.535748482F,
    0.464188606F, 0.497194976F, 0.504703164F, 0.564794362F, 0.569056213F,
    0.648744524F, 0.641538918F, 0.438143879F, 0.203294665F, 0.250606209F,
    0.271041393F, 0.278585762F, 0.392930627F, 0.0962901264F, 0.306592762F,
    0.427215159F, 0.331904978F, 0.560076654F, 0.660027444F, 0.617422521F,
    0.571456134F, 0.560560346F, 0.427240551F, 0.316384017F, 0.389279F,
    0.218903482F, 0.366780967F, 0.373727083F, 0.238840818F, 0.468676746F,
    0.512435436F, 0.55697149F, 0.446255237F, 0.517826498F, 0.651491106F,
    0.565305054F, 0.515428722F, 0.411574F, 0.364097297F, 0.386241555F,
    0.285394043F, 0.309091568F, 0.200203955F, 0.385758549F, 0.510658264F,
    0.563458145F, 0.553617477F, 0.438469797F, 0.502862215F, 0.685667336F,
    0.541185737F, 0.52667731F, 0.405547321F, 0.427541196F, 0.485501558F,
    0.336746037F, 0.3467713F, 0.304294914F, 0.301513463F, 0.483992428F,
    0.61684984F, 0.63828361F, 0.501134217F, 0.406493902F, 0.393756449F,
    0.506867051F, 0.517800272F, 0.377945334F, 0.432415277F, 0.466570556F,
    0.363590151F, 0.326571941F, 0.285144866F, 0.27506882F, 0.487723976F,
    0.484516174F, 0.350942552F, 0.442714602F, 0.482862175F, 0.371164888F,
    0.521671295F, 0.488904744F, 0.408195466F, 0.383968472F, 0.296159089F,
    0.283609658F, 0.268013746F, 0.338131368F, 0.165717214F, 0.385933369F,
    0.477493F, 0.430029452F, 0.449852139F, 0.473768324F, 0.510699332F,
    0.490644783F, 0.62623F, 0.364249319F, 0.274101436F, 0.250784516F,
    0.308335304F, 0.274206817F, 0.29249227F, 0.209715F, 0.347273707F,
    0.445426941F, 0.442095459F, 0.488573402F, 0.400363952F, 0.492306292F,
    0.638446033F, 0.573763669F, 0.420014501F, 0.387810975F, 0.289961219F,
    0.365061611F, 0.327600569F, 0.187102646F, 0.125631705F, 0.121965051F,
    0.30683428F, 0.335931778F, 0.383028895F, 0.500308931F, 0.457101F,
    0.553214073F, 0.415947467F, 0.426600605F, 0.404477268F, 0.281132311F,
    0.258974F, 0.224946886F, 0.285544217F, 0.140270233F, 0.179999575F,
    0.193721801F, 0.429003924F, 0.417212367F, 0.421523571F, 0.329183966F,
    0.465584695F, 0.346550077F, 0.361085236F, 0.251373917F, 0.272058219F,
    0.224687755F, 0.204461992F, 0.214699626F, 0.196025103F, 0.257111F,
    0.0885170102F, 0.184347257F, 0.349387705F, 0.351232111F, 0.383124501F,
    0.342655867F, 0.504709899F, 0.308243364F, 0.310555518F, 0.231344849F,
    0.273328F, 0.23110792F, 0.139352441F, 0.164779186F, 0.302659333F,
    0.194256842F, 0.368608326F, 0.396195918F, 0.313228905F, 0.43912524F,
    0.401856184F, 0.268323779F, 0.272064447F, 0.261030495F, 0.215714F,
    0.269945353F, 0.297433704F, 0.241744667F, 0.105089568F, 0.318859249F,
    0.294125021F, 0.530399263F, 0.406675369F, 0.443848938F, 0.525340259F,
    0.687816501F, 0.583788157F, 0.35496673F, 0.355634511F, 0.323309571F,
    0.184181631F, 0.353355676F, 0.229913652F, 0.216270268F, 0.26571843F,
    0.335045159F, 0.405139238F, 0.48177439F, 0.469702899F, 0.389096797F,
    0.464123249F, 0.585272551F, 0.352781981F, 0.293189377F, 0.290509731F,
    0.275561422F, 0.263590336F, 0.220996737F, 0.189013198F, 0.166247189F,
    0.245077118F, 0.42359215F, 0.357131958F, 0.395445466F, 0.435493439F,
    0.431458712F, 0.406282336F, 0.396752596F, 0.347103119F, 0.239855781F,
    0.197317943F, 0.242752F, 0.303268194F, 0.170425326F, 0.183541626F,
    0.128786132F, 0.199147329F, 0.463696063F, 0.428774357F, 0.376905024F,
    0.338273287F, 0.387152314F, 0.244006559F, 0.272904813F, 0.288420886F,
    0.212464064F, 0.234610781F, 0.278884917F, 0.253895551F, 0.216369107F,
    0.16678305F, 0.314790845F, 0.322473556F, 0.479684949F, 0.371406019F,
    0.343777478F, 0.37422502F, 0.308839262F, 0.33034721F, 0.248168498F,
    0.333828121F, 0.278447181F, 0.214880601F, 0.176954776F, 0.0859544054F,
    0.123165958F, 0.360288262F, 0.304133177F, 0.246851131F, 0.405260712F,
    0.328657508F, 0.38614735F, 0.349459708F, 0.279476404F, 0.239808798F,
    0.204765946F, 0.269454271F, 0.194676369F, 0.257303685F, 0.21564123F,
    0.247862786F, 0.319118381F, 0.401834249F, 0.386930585F, 0.192146212F,
    0.292936713F, 0.278616965F, 0.228684619F, 0.264332443F, 0.215466335F,
    0.287091643F, 0.176165015F, 0.090398185F, 0.266942322F, 0.162824348F,
    0.112251647F, 0.210920781F, 0.293508053F, 0.292003423F, 0.174420953F,
    0.232966796F, 0.237773463F, 0.30760771F, 0.176108181F, 0.164646104F,
    0.162436098F, 0.256026775F, 0.0987772644F, 0.256149203F, 0.395304948F,
    0.479829192F, 0.497719288F, 0.291229665F, 0.258182198F, 0.172262F,
    0.209425807F, 0.196253985F, 0.127497673F, 0.189766183F, 0.156456F,
    0.0716141313F, 0.160463333F, 0.143498987F, 0.622135103F, 0.733088374F,
    0.565630317F, 0.551228464F, 0.399123222F, 0.219399109F, 0.0785215124F,
    0.0786023736F, 0.14447993F, 0.0995590836F, 0.149989337F, 0.163618848F,
    0.192385316F, 0.103688352F, 0.112076506F, 0.792701483F, 0.74579972F,
    0.749048173F, 0.628883839F, 0.533225477F, 0.341232061F, 0.0973513275F,
    0.109001875F, 0.144775555F, 0.182056427F, 0.150972173F, 0.245424062F,
    0.0608802959F, 0.126849711F, 0.216758475F, 0.895633161F, 0.815802097F,
    0.679516912F, 0.682133555F, 0.581508398F, 0.291954696F, 0.1589531F,
    0.139156371F, 0.207403705F, 0.132526323F, 0.20739089F, 0.157966271F,
    0.0936969891F, 0.178263083F, 0.23885417F, 0.861511707F, 0.858840585F,
    0.768850446F, 0.688622117F, 0.592543721F, 0.372317F, 0.36949411F,
    0.167333186F, 0.322253F, 0.333832324F, 0.30593884F, 0.267819375F,
    0.176164478F, 0.251265138F, 0.246079803F, 0.899664581F, 0.892829776F,
    0.805212379F, 0.799212456F, 0.616411388F, 0.596234798F, 0.348740429F,
    0.229810596F, 0.354520589F, 0.331164867F, 0.285171419F, 0.211228549F,
    0.193400025F, 0.171894222F, 0.424278796F, 0.941744447F, 0.893073142F,
    0.832387567F, 0.729730368F, 0.655224562F, 0.544021428F, 0.395646602F,
    0.467508018F, 0.424280494F, 0.385968775F, 0.408720851F, 0.285462528F,
    0.218466222F, 0.369510055F, 0.413198769F };

    __attribute__ ((section(".heapsram"))) const float fv3[16] = { 60.7386436F, 60.7374306F, 0.0107773514F,
    0.0107771354F, 90.2136688F, 90.2118683F, 0.00870365929F, 0.00870348606F,
    87.5330734F, 87.5313187F, 0.0155322747F, 0.0155319637F, 74.803833F,
    74.8023376F, 0.0358732603F, 0.0358725451F };


/////////////////////////////////////////////////////////
// subfunctions
/////////////////////////////////////////////////////////

void mlBilat(const float img[225], const float fv0[2], float filt[225])
{
  int k;
  int x;
  int y;
  unsigned int qY;
  int varargin_2;
  int ux;
  int ly;
  int uy;
  float nrm;
  int x_;
  int d_x;
  int y_;
  float a;
  float w;

  float b_std[2];

  b_std[0] = fDiv(-0.5F, (fv0[0] * fv0[0]));
  b_std[1] = fDiv(-0.5F, (fv0[1] * fv0[1]));
  
  for (x = 0; x < 15; x++) {
    for (y = 0; y < 15; y++) {
      /*  calculate bounding box and clip at image border */
      k = x + 1;
      qY = k - 3U;
      if (qY > (unsigned int)k) {
        qY = 0U;
      }

      varargin_2 = (int)qY;
      k = (int)((x + 1) + 3U);
      if (15U <= (unsigned int)k) {
        ux = 15;
      } else {
        ux = k;
      }

      k = y + 1;
      qY = k - 3U;
      if (qY > (unsigned int)k) {
        qY = 0U;
      }

      k = (int)qY;
      if (1 < k) {
        ly = k;
      } else {
        ly = 1;
      }

      k = (int)((y + 1) + 3U);
      if (15U <= (unsigned int)k) {
        uy = 15;
      } else {
        uy = k;
      }

      nrm = 0.0F;
      filt[y + 15 * x] = 0.0F;
      if (1 < varargin_2) {
        x_ = varargin_2;
      } else {
        x_ = 1;
      }

      while ((unsigned int)x_ <= (unsigned int)ux) {
        /* x distance */
        k = x + 1;
        qY = (unsigned int)k - x_;
        if (qY > (unsigned int)k) {
          qY = 0U;
        }

        k = (int)qY;
        d_x = k * k;
        for (y_ = ly; (unsigned int)y_ <= (unsigned int)uy; y_++) {
          /*  y distance */
          k = y + 1;
          qY = (unsigned int)k - y_;
          if (qY > (unsigned int)k) {
            qY = 0U;
          }

          k = (int)qY;

          /*  photometric distance */
          a = img[y + 15 * x] - img[(y_ + 15 * (x_ - 1)) - 1];
          w = fExp((float)(d_x + k * k) * b_std[0] + a * a * b_std[1]);
          nrm += w;
          filt[y + 15 * x] += w * img[(y_ + 15 * (x_ - 1)) - 1];
        }

        x_++;
      }

      filt[y + 15 * x] = fDiv(filt[y + 15 * x], nrm);
    }
  }
}

float sum(const float x[225])
{
  float y;
  int k;
  y = x[0];
  for (k = 0; k < 224; k++) {
    y += x[k + 1];
  }

  return y;
}

float var(const float x[225])
{
  float y;
  int ix;
  float xbar;
  int k;
  float r;
  ix = 0;
  xbar = x[0];
  for (k = 0; k < 224; k++) {
    ix++;
    xbar += x[ix];
  }

  xbar = fDiv(xbar,225.0F);
  ix = 0;
  r = x[0] - xbar;
  y = r * r;
  for (k = 0; k < 224; k++) {
    ix++;
    r = x[ix] - xbar;
    y += r * r;
  }

  y = fDiv(y,224.0F);
  return y;
}

boolean_T checkRes(const float check[2], const float golden[4])
{
  boolean_T pass = true;
  boolean_T flag = true;
  int k;
  
  for (k = 0; k < 2; k++) {
    flag = true;
    flag = flag && (check[k] <= golden[k << 1]);
    flag = flag && (check[k] >= golden[1 + (k << 1)]);
    printErrors(!flag, k, check[k] ,golden[k<<1] ,golden[1+(k<<1)]);
    pass = pass && flag;
  }
  
  return pass;
}

/////////////////////////////////////////////////////////
// main testing function 
/////////////////////////////////////////////////////////
int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  int coreid;
  int k;
  float y[225];
  
  boolean_T pass;
  float tmp[2];
  
  /////////////////////////////////////////////////////////
  // main test loop 
  // each core loops over a kernel instance
  /////////////////////////////////////////////////////////

  coreid = get_core_id();

  printf("starting %d kernel iterations... (coreid = %d)\n",KERNEL_ITS,coreid);

  if (coreid>3)
    coreid=coreid-4;

  synch_barrier();

  perf_begin();

  for(k = 0; k < 1; k++)
    //  for(k = 0; k < getKernelIts(); k++)
  {
    // matlab kernel
    mlBilat(*(float (*)[225])&fv2[225 * coreid], fv1, y);
  }

  synch_barrier();
  perf_end();

  /////////////////////////////////////////////////////////
  // check results
  /////////////////////////////////////////////////////////

  tmp[0] = sum(y);
  tmp[1] = var(y);

  pass = checkRes(tmp, *(float (*)[4])&fv3[coreid << 2]);
    
  flagPassFail(pass, get_core_id());
  
  synch_barrier();
   
/////////////////////////////////////////////////////////
// synchronize and exit
/////////////////////////////////////////////////////////

  return !pass;
}
