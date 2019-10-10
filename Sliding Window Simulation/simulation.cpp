#include<iostream>
#include<cstring>
#include<algorithm>
#include<fstream>
#include<sstream>
#include<cmath>
#include<ctime>
using namespace std;

// this poly is x^16 + x^15 + x^2 + 1
// crc (remainder) can be directly xor'ed with it
#define POLY                0x18005
#define FCS_LEN             16
#define RETRY               3
#define FRAME_SIZE          100
#define NAK_PROBABILITY     5       // probability that receiver will send NAK. out of 100
#define NO_SIG_PROBABILITY  60      // probability that receiver won't send signal. out of 100;
#define ERROR_PROBABILITY   15

// function declarations
int CRC16(string bitSteream, int length);
string int_to_bitstring(int integer);
string int_to_bitstring_padded(int integer, int size);

// structure for sending signal
struct signal
{
    string type;
    int num;
};

class Receiver
{
public:
    int k, w, mod;    // window size
    int winSt, winEn, winCur;
    Receiver(int frameSize, int windowSize)
    {
        winSt = 0;
        winEn = windowSize - 1;
        winCur = 0;
        w = windowSize;
        k = frameSize;
        mod = pow(2, k);
    }
    void printWindow()              // print current window
    {
        cout<<"Receiver Window:  ";
        for (int j=winSt-2; j<=winEn; j++)
        {
            if (j<0) continue;
            if (j==winSt) cout<<"| ";           // start of window. draw a bar
            if (winCur==j) cout<<"[";           // start of current frame. draw opening bracket
            cout<<j%mod;
            if (j==winEn && winCur>winEn) cout<<" [";
            if (j==winEn) cout<<"] ";           // end of window. draw closing bracket
            else cout<<" ";
        }
        cout<<endl;
    }
    struct signal send(string frame, int num)       // method to send frame to receiver
    {
        cout<<"\nRECEIVER"<<endl;
        cout<<"========"<<endl;

        struct signal ret;

        if (frame == "EOT")             // EOT. Send ACK
        {
            cout<<"Received EOT!"<<endl;
            cout<<"Sending ACK!"<<endl;
            ret.type = "ACK";
            ret.num = num + 1;
            printWindow();
            return ret;
        }

        int check = CRC16(frame, frame.length());

        if ( check != 0)
        {
            cout<<"Received Damaged Frame!"<<endl;
            cout<<"Sending NAK!"<<endl;
            ret.type = "NAK";
            ret.num = num;

            winSt = num;
            winCur = winSt;
            winEn = winSt + w - 1;
            printWindow();
            return ret;
        }
        else
        {
            int success_probability = rand()%100;

            if (success_probability < NAK_PROBABILITY)        // send NAK
            {
                cout<<"Sending NAK"<<endl;

                int r = winSt + rand()%(winCur-winSt+1);        // getting a random frame between
                                                                // start and current position of window
                ret.type = "NAK";
                ret.num = r; //num. frame num

                winSt = r;  // num.     frame num
                winCur = winSt;
                winEn = winSt + w - 1;
            }
            else if (success_probability < NO_SIG_PROBABILITY)          // don't send any signal
            {
                cout<<"........"<<endl;
                ret.type = "-1";
                ret.num = num;
                winCur++;
                if (winCur == winEn + RETRY)            // window is full. Send Forced ACK
                {                                   // receiver already asked for ACK several times
                    cout<<"Sending  ACK!"<<endl;
                    ret.type = "ACK";
                    ret.num = num + 1;

                    winSt = num + 1;                // update window to start from next frame
                    winCur = winSt;
                    winEn = winSt + w - 1;
                }
            }
            else                                // send ACKs
            {
                cout<<"Sending  ACK"<<endl;
                ret.type = "ACK";
                ret.num = num + 1;

                winSt = num + 1;
                winCur = winSt;
                winEn = winSt + w - 1;
            }
            printWindow();
            return ret;
        }
    }
};

int main()
{
    srand(time(NULL));

    ifstream data_file;
    data_file.open("test_data.txt");        // opening file to get bit stream

    stringstream ss;
    string data_stream;
    char buf[1000];                     // temporary buffer

    int k, w, transmission_num = 1, frame_size = FRAME_SIZE, datafield_size, frame_num;
    int fcs_int, winSt, winEn, mod;
    string data, frame_check, frame_final, fcs_string;
    string frame_num_in_bit;

    while (data_file >> data_stream)
    {
        k = 2;
        w = 3;
        cout<<"\t\t --------------------------------"<<endl;
        cout<<"\t\t|  Transmission Simulation : #"<<transmission_num++<<"  |"<<endl;
        cout<<"\t\t --------------------------------"<<endl<<endl;

        cout<<"Default:  k="<<k<<"  w="<<w<<"  Frame Size="<<frame_size<<endl<<endl;
        cout<<"1: Simulate with Default Parameters"<<endl;
        cout<<"2: Simulate with Given Parameters"<<endl;

        int choice;
        cin>>choice;

        if (choice == 2)
        {
            cout<<"Enter  k: ";
            cin>>k;
            cout<<"Enter  w: ";
            cin>>w;
            cout<<"Enter Frame Size: ";
            cin>>frame_size;
        }
        cout<<endl;

        mod = pow(2, k);

        if (w >= mod)            // w exceeds limit;
        {
            cout<<"ERROR: Window Too Large"<<endl;
            exit(-1);
        }

        winSt = 0;
        winEn = w-1;

        Receiver receiver(k, w);           // creating a receiver

        frame_num = 0;                     // initialize frame num to 0
        datafield_size = frame_size - FCS_LEN - k;       // calculate size of data field

        ss.clear();
        ss.str("");
        ss<<data_stream;

        // Create all frames
        int i = 0;
        string sendBuf[1000];

        while (ss.get(buf, datafield_size+1) )          // get datafield sized chunk from bit stream
        {
            data = string(buf);
            //cout <<"Data: "<<data<<endl;

            frame_num_in_bit = int_to_bitstring_padded(frame_num, k);   // convert frame num in bit and pad with zeroes
            frame_check = frame_num_in_bit + data + int_to_bitstring_padded(0, FCS_LEN);    // dummy frame for calculating fcs

            fcs_int = CRC16(frame_check, frame_check.length());         // FCS in integer
            fcs_string = int_to_bitstring_padded(fcs_int, 16);          // Convert FCS to bit and pad with zero if needed

            frame_final = frame_num_in_bit + data + fcs_string;         // Final Frame with FCS added
            //cout<<"Frame with FCS: "<<frame_final<<endl;

            sendBuf[i++] = frame_final;
            frame_num = (frame_num+1)%mod;
        }
        sendBuf[i] = "EOT";            // set last frame to EOT
        int bufSize = i;
        bool windowFull = false;

        for (i=0; i < bufSize+1; i++)
        {

            cout<<"Sender Window:  ";
            for (int j=winSt-2; j<=winEn+2; j++)
            {
                if (j<0) continue;
                if (j==winSt) cout<<"| ";
                if (i==j && windowFull==false) cout<<"[";
                cout<<j%mod;
                if (i==j && windowFull==true) cout<<" [";
                if (j==winEn) cout<<"] ";
                else cout<<" ";
            }
            cout<<endl;

            if (windowFull==false) {
                    cout<<"\n\t\t ---------------------"<<endl;
                    cout<<"\t\t|  SENDING FRAME: #"<<i%mod<<"  |"<<endl;
                    cout<<"\t\t ---------------------";
            }

            string frame_to_be_sent = sendBuf[i];

            int success_probability = rand()%100;
            if (success_probability < ERROR_PROBABILITY)            // Damage the frame
            {
                frame_to_be_sent[4] = (frame_to_be_sent[4]=='1')?'0':'1';
                frame_to_be_sent[frame_size-5] = (frame_to_be_sent[frame_size-5]=='1')?'0':'1';
            }

            struct signal res = receiver.send(frame_to_be_sent, i);         // get resultant signal after sending frame

            cout<<"\nSENDER"<<endl;
            cout<<"======"<<endl;

            if (res.type != "-1")
            {
                cout<<"RECEIVED: "<<res.type<<" "<<res.num%mod<<endl;
                windowFull = false;
            }
            else cout<<"NO SIGNAL RECEIVED!"<<endl;

            if (res.type == "ACK")
            {
                winSt = res.num;                // Got ACK. So, adjust Window.
                winEn = winSt + w - 1;
            }
            else if (res.type == "NAK")
            {
                winSt = res.num;            // Got NAK. Start Window from the Errored Frame.
                winEn = winSt + w - 1;
                i = winSt;
                i--;                        // Send That Errored Frame again
            }
            else                                // No signal received
            {
                if (i == winEn)                 // all frames of window sent
                {
                    cout<<"* All Frames Sent!!"<<endl;
                    cout<<"* Waiting For ACK/NAK"<<endl;
                    windowFull = true;
                    i--;
                }
            }
        }
        cout<<"Final Window:  ";
        for (int j=winSt-2; j<=winEn; j++)
        {
            if (j<0) continue;
            if (j==winSt) cout<<"| ";
            if (i==j && windowFull==false) cout<<"[";
            cout<<j%mod;
            if (j==winEn) cout<<"] ";
            else cout<<" ";
        }
        cout<<endl<<endl;
    }
}


// function definitions
int CRC16(string stream, int len)
{
    int crc = 0;                            // init crc as 0
    int check;
    for (int i=0; i<len; i++)
    {
        check = crc>>(FCS_LEN-1) & 1;       // check leftmost bit of crc
        crc <<= 1;                          // left shift crc
        crc ^= (stream[i]-'0');             // append 1 bit from data stream with crc

        if (check==1)
        {
            crc ^= POLY;                    // xor crc with polynomial
        }
    }
    return crc;
}

string int_to_bitstring(int n)
{
    string res = "";
    while(n!=0)
    {
        res += ((n%2) + '0');
        n /= 2;
    }

    reverse(res.begin(), res.end());
    return res;
}
string int_to_bitstring_padded(int n, int size)
{
    string str = int_to_bitstring(n), zero = "";
    int x = size - str.length();
    if (x>0)
    {
        for (int i=0; i<x; i++) zero += "0";
        return zero + str;
    }
    else
    {
        return str;
    }
}
