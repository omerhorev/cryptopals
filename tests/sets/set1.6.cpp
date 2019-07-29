//
// Created by omerh on 26/07/2019.
//

#include <gtest/gtest.h>
#include <array>
#include <tuple>
#include <utils/debug.h>
#include <utils/hamming-distance.h>
#include <breaks/xor-break.h>
#include <model/xor-byte-key.h>

static const char vector_cipher[] = "1d421f4d0b0f021f4f134e3c1a69651f491c0e4e13010b074e1b01164536001e01496420541d1d4333534e6552060047541c0d454d07040c53123c0c1e08491a09114f144c211a472b00051d47591104090064260753003716060c1a17411d015254305f0020130a05474f124808454e653e160938450605081a46074f1f59787e6a62360c1d0f410d4806551a1b001d4274041e01491a091102527a7f4900483a001a13491a4f45480f1d0d53043a015219010b411306004c315f53621506070907540b17411416497933350b1b01050f46074f1d4e784e48275204070c4558480841004f205408740b1d194902000e165c00523069651f4902025400010b074e021053012610154d0207021f4f1b4e78306936520a010954060709534e021053083b100605490f0f104f3b003a5f472b1c4964334f540210531a4f051611740c064d0f020e0343524c3d4e002f0b490d084e170d15541d4f0e1c455e280b4d190112070a5553784e4f6206010b47531d0c0000170a051f0c3a425e4d2e0141220e1c493456416235064f472a7e3b084f011b0153423704071e0c4e151c0e06072b1a542a17491906595421455707030553073145782c070a411d095259374f00261d07491300130113454e0e491704390b5e4d1d06041a4f78773043003b1d1c4e1454151a0c4e494f080745390052673a0141130a0600375c4662550a0f125311482c000d000707173b095219010b41071b13473d1a2a161a0c1c020707480b4f4e0b0000163d0b554d08020d1b181744783069651f490709001911454f190149030d3516174d633a09114f15492a56492701491d06000d4811480b16491f0a220052000c4e001a0b5254305b54621b1a4e084b5462244e0a4f205306350b5209080002114f10452c4e4530521d06064e54090b594e040017453a42521d050f18546578732c5b4727525b4e4a0d543100414e1b0116453b0b174d100f465418134e365b002e1b1a1a024e541c0a0064261d5416740a140b490318540717413c1a532d52050b1300000000000c0a08074524091314491a0906000747301a2a111d49274743150645461b0102530c2045071d490f0f104f1f41335f002b06491d08551a0c454701000d536f654840405a4e381b4f5f0d78714e2d11024e084e541b0a4d0b4f1e1c0a3045782b061c4113001d447856552119454e2e0018010e454e021053173c1c1f081a4e00001d1d433155553152633d1250111a0641020e0f0104330c1e041a1a08170a0a50315b4c2b16060d0e4f011b452a27480453043a45170b0f0b02004f134e3c1a542a131d4e1e4f01480641004f0b1611746f3b4d0a0f0f541b134b3d1a416214051747471d1a09000f010d5308350e174d010b1354181754761a2a483b4e03474c1d0300003d0e04000a3a455f40493d00191c1d4e784e4f62360c020e4c1500452a3a070c010073165203064e0511010b49361d0c622b061b4743150645541c1649070a740d13030e4e6b361a0600215555651e054e0c45111845541c16001d4274111d4d0e0b1554020b002b4e592e1749642856111a4541000b491c1331175e4d191c00171b1b433d1a4d23190c1d4750111a03450d1b49792721115203061a411d095259374f073017490f474c1b0903451c4149796f0d0a074a050241130a06003655572a171b0b4b001a074550020e0a1649740b1d4d1d070c1143524e371a472b00051d472a27070a4e4e4244532a3c451f1449290e10435248375745201d0d174b000d0710001e1d06110436090b4d0c0f1554652150395d4827061d0747571d1c0d000f4f1a030a3b0b534d2a010c114f1d4e785b4e26521a0f1e001d1c440064653f3a357a45240c07070d180e52693b5f003b17194247591118490027480453063b081b034e4e09151d160034534b2752084e15481d060a00642607070a2c0c110c1d070f134f014f78434f37521a1a0647130d17000206021645354505040701417e3c1d00284f4e2901491d134f044811521706071445350b164d0e0713184f0154374a00210010070907546233410006051f04742c1108490712541c174c34534e6552080003000d0710001e0a0603093145131f0c4e0301161b4e7f1a2a6531081b1445541f0d594e1b0116453217170c021d41151d1700325543291b0749474c1d0300002d1d08091c74221e180c4e6b39000449361d00231c0d4e00521b07134900484907172d0c1c0a491a0e541c1b4e3f1a412e1d0709472a350409001a071b1c10330d5219010b41130717542c55002500060111491a4f455406061a530d3117174d1a010f134f786e374d003b1d1c49154554090841140a0d53072d4506050c4e373d3f5250374953275c49646d73000d155007014e53163b451a0c1b0a4118061945785b0005171b03064e5426045a074f632011351706010c0a4116165254305f0020131a0b14001c01115407014e5302260a07030d4e6b200717523d1d53621c064e13521d1815490048491c0b74081b030c42413d481f00324f5336520e0b13541d0642000a001e1d455e36020c1b0500190e06493b16000b55044e0f411a0f0c4e494f1d1a023c115201000504540e5246395441361b0a4e6d791b1d45541c0e19030030451f0849010f170a5241365e000b521d060855130011001a070807455e3c1d1849030813070600305b562752001a472a270745531a0a1953013b121c4d0800055403174e3c1a4d275210011252540d04524e654e4b5c740c1c4d04174100061f45791a792d07454e401944480c534e0210531c3104004349646b2d0007072a5f0035170805024e1d064200080e1a0749743c3d4c490f0f104f3b003b5b4e62060c020b001d1c452a37001c0145360a16144e1d41130a0654315407621a061a4b00070749001d00493a4537041c4d1a0304180352492c1a2a111d490a084e531c45420b4f04120174041c09490a0e1a4806003a5f0031130d4e6d07370910530b4f1d1b0074090b1f000d12540d174c3754476206064e2e6331444579011a4910043a45110c050241190a5264395e00482b061b4052114815491a0c011a0b7345134d0f0715584f014f7849542702490c06431f48044e0a4f0c1d012117174d632204004f06483d1a572b060a0647441b0b114f1c43493a0631495209064e151c0a524439544327521d014743011a0000643c0653063b08174d1c1e4117031d533d1a412c16490a084e531c45420b4f1a02103517174d63370e014f05413654416210081a134c114808454e424453243a1c0604040b4d540e1c592f5245301749646d791b1d455406001c140d20450605081a413d4f05412b1a572713024247621b11490017001c541731451608080a41031d1d4e3f1a2a111d490d084d11480a4e424f0c0500261c10020d1741150116002b534e25521d060e53541b0a4e094f637936351c5240444e31180e0b002c524136520f1b094b0d4808551d060a5336351c5e4d0e014103071b543d1a422d0b454e004f541f0d491a0a49110a2d451502496411180e0b002c524136520f1b094b0d4808551d060a53223b450505001a04540d1d59741a472d521e060e541148074f174349140a746f3e0c104e051b181c003954446210060100491148044e0a4f191f042d450605081a41121a1c4b211a4d3701000d47541d04090017001c53013d005c4d636431180e0b002c524136520f1b094b0d4808551d060a53263b08174d06004d542c1d4d3d1a4f2c5e4902025454050000060a0801455e351e0c104e151c0e06003e4f4e290b490312531d0b455706061d1645360a0b4d100114541c13597853546e521a0f1e001d1c452a3e03080a45200d13194908141a040b00354f532b11492f474c1d1c114c0b4f051c103000004d0701165465224c394300361a081a474601060e594e021c000c3749521a010715114f104f211a632d1f0c4e084e5848264f030a491c0b78453102040b411b01522a0856413b521d060654540e104e0516491e10270c114d63";
static const char vector_key[] = "Terminator X: Bring the noise";
static const char vector_plain[] = "I'm back and I'm ringin' the bell \n"
                                   "A rockin' on the mike while the fly girls yell \n"
                                   "In ecstasy in the back of me \n"
                                   "Well that's my DJ Deshay cuttin' all them Z's \n"
                                   "Hittin' hard and the girlies goin' crazy \n"
                                   "Vanilla's on the mike, man I'm not lazy. \n"
                                   "\n"
                                   "I'm lettin' my drug kick in \n"
                                   "It controls my mouth and I begin \n"
                                   "To just let it flow, let my concepts go \n"
                                   "My posse's to the side yellin', Go Vanilla Go! \n"
                                   "\n"
                                   "Smooth 'cause that's the way I will be \n"
                                   "And if you don't give a damn, then \n"
                                   "Why you starin' at me \n"
                                   "So get off 'cause I control the stage \n"
                                   "There's no dissin' allowed \n"
                                   "I'm in my own phase \n"
                                   "The girlies sa y they love me and that is ok \n"
                                   "And I can dance better than any kid n' play \n"
                                   "\n"
                                   "Stage 2 -- Yea the one ya' wanna listen to \n"
                                   "It's off my head so let the beat play through \n"
                                   "So I can funk it up and make it sound good \n"
                                   "1-2-3 Yo -- Knock on some wood \n"
                                   "For good luck, I like my rhymes atrocious \n"
                                   "Supercalafragilisticexpialidocious \n"
                                   "I'm an effect and that you can bet \n"
                                   "I can take a fly girl and make her wet. \n"
                                   "\n"
                                   "I'm like Samson -- Samson to Delilah \n"
                                   "There's no denyin', You can try to hang \n"
                                   "But you'll keep tryin' to get my style \n"
                                   "Over and over, practice makes perfect \n"
                                   "But not if you're a loafer. \n"
                                   "\n"
                                   "You'll get nowhere, no place, no time, no girls \n"
                                   "Soon -- Oh my God, homebody, you probably eat \n"
                                   "Spaghetti with a spoon! Come on and say it! \n"
                                   "\n"
                                   "VIP. Vanilla Ice yep, yep, I'm comin' hard like a rhino \n"
                                   "Intoxicating so you stagger like a wino \n"
                                   "So punks stop trying and girl stop cryin' \n"
                                   "Vanilla Ice is sellin' and you people are buyin' \n"
                                   "'Cause why the freaks are jockin' like Crazy Glue \n"
                                   "Movin' and groovin' trying to sing along \n"
                                   "All through the ghetto groovin' this here song \n"
                                   "Now you're amazed by the VIP posse. \n"
                                   "\n"
                                   "Steppin' so hard like a German Nazi \n"
                                   "Startled by the bases hittin' ground \n"
                                   "There's no trippin' on mine, I'm just gettin' down \n"
                                   "Sparkamatic, I'm hangin' tight like a fanatic \n"
                                   "You trapped me once and I thought that \n"
                                   "You might have it \n"
                                   "So step down and lend me your ear \n"
                                   "'89 in my time! You, '90 is my year. \n"
                                   "\n"
                                   "You're weakenin' fast, YO! and I can tell it \n"
                                   "Your body's gettin' hot, so, so I can smell it \n"
                                   "So don't be mad and don't be sad \n"
                                   "'Cause the lyrics belong to ICE, You can call me Dad \n"
                                   "You're pitchin' a fit, so step back and endure \n"
                                   "Let the witch doctor, Ice, do the dance to cure \n"
                                   "So come up close and don't be square \n"
                                   "You wanna battle me -- Anytime, anywhere \n"
                                   "\n"
                                   "You thought that I was weak, Boy, you're dead wrong \n"
                                   "So come on, everybody and sing this song \n"
                                   "\n"
                                   "Say -- Play that funky music Say, go white boy, go white boy go \n"
                                   "play that funky music Go white boy, go white boy, go \n"
                                   "Lay down and boogie and play that funky music till you die. \n"
                                   "\n"
                                   "Play that funky music Come on, Come on, let me hear \n"
                                   "Play that funky music white boy you say it, say it \n"
                                   "Play that funky music A little louder now \n"
                                   "Play that funky music, white boy Come on, Come on, Come on \n"
                                   "Play that funky music \n";

float break_cyclic_xor_encryption_by_key_length(const unsigned char cipher[], size_t length, size_t key_length,
                                                unsigned char o_key[])
{
    const size_t number_of_split_buffers = key_length;
    const size_t split_buffer_max_size = (length / number_of_split_buffers) + 1;

    size_t split_buffer_write_index[number_of_split_buffers];
    unsigned char split_buffer[number_of_split_buffers][split_buffer_max_size];

    memset(split_buffer_write_index, 0, sizeof(split_buffer_write_index));
    memset(split_buffer, 0, sizeof(split_buffer));

    for (auto i = 0; i < length; ++i)
    {
        auto k = (i % number_of_split_buffers);

        split_buffer[k][split_buffer_write_index[k]] = cipher[i];
        split_buffer_write_index[k]++;
    }

    breaks::xor_break b;

    float sum_score = 0;

    for (int j = 0; j < number_of_split_buffers; ++j)
    {
        unsigned char key_char;
        float score;

        b.single_xor_cipher_break(split_buffer[j], split_buffer_write_index[j], key_char, score);

        o_key[j] = key_char;

        sum_score += score;
    }

    return (sum_score / number_of_split_buffers);
}

TEST(sets, set_1_6)
{
    const size_t compare_blocks_count = 4;
    const size_t max_key_size_possible = 40;
    const size_t cipher_length = 2876;
    const size_t key_lengths_to_check = 5;

    std::array<char, cipher_length> c = h2b(vector_cipher);
    auto cipher = (const unsigned char *) &c;
    std::array<std::tuple<float, unsigned int>, max_key_size_possible> key_hamming_distances;
    float highest_score = 0;
    size_t selected_key_length = 0;
    unsigned char key[cipher_length];
    unsigned char temp_key[cipher_length];
    char plain_text[cipher_length];

    memset(plain_text, 0, sizeof(plain_text));

    // First of all, lets figure out the possible key sizes
    for (unsigned int i = 0; i < max_key_size_possible; i++)
    {
        size_t block_size = i;
        float d = 0;

        // Calculate average hamming distance...
        for (auto j = 0; j < compare_blocks_count; j++)
        {
            const unsigned char *b1 = cipher + (block_size * (j * 2));
            const unsigned char *b2 = cipher + (block_size * (j * 2 + 1));

            d += utils::hamming_distance::calculate(b1, b2, block_size);
        }

        d /= compare_blocks_count;
        d /= block_size;

        std::get<unsigned int>(key_hamming_distances[i]) = i;
        std::get<float>(key_hamming_distances[i]) = d;
    }

    std::sort(key_hamming_distances.begin(), key_hamming_distances.end(), [](auto a, auto b)
    {
        return std::get<float>(a) < std::get<float>(b);
    });

    // Lets try to break each key
    for (auto k = 0; k < key_lengths_to_check; ++k)
    {
        auto key_length = std::get<unsigned int>(key_hamming_distances[k]);
        auto score = break_cyclic_xor_encryption_by_key_length(cipher,
                                                               cipher_length,
                                                               key_length,
                                                               temp_key);

        if (score > highest_score)
        {
            selected_key_length = key_length;
            memset(key, 0, sizeof(key));
            memcpy(key, temp_key, key_length);
            highest_score = score;
        }
    }

    model::xor_byte_key::decrypt(cipher, (unsigned char *) plain_text, cipher_length, key, selected_key_length);

    ASSERT_EQ(selected_key_length, strlen(vector_key));
    ASSERT_TRUE(memcmp(vector_key, key, selected_key_length) == 0);

    ASSERT_EQ(cipher_length, strlen(vector_plain));
    ASSERT_TRUE(memcmp(vector_plain, plain_text, cipher_length) == 0);
}