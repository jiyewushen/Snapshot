package com.floatWind.snap;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.ContentResolver;
import android.content.res.AssetFileDescriptor;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.ParcelFileDescriptor;
import android.util.Log;
import android.widget.ImageView;

import com.ffmpeg.VideoSnap;

import org.apache.commons.io.IOUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MediaMetadataRetrieverJNI";
    private ImageView show;
    private volatile Bitmap bitmap;
    @SuppressLint("HandlerLeak")
    private Handler handler = new Handler() {
        @Override
        public void handleMessage(@NonNull Message msg) {
            if (msg.what == message) {
                show.setImageBitmap(bitmap);
            } else {
                super.handleMessage(msg);
            }
        }
    };
    private int message = 0x123;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        show = findViewById(R.id.show);
    }

    @Override
    protected void onResume() {
        super.onResume();
        new Thread() {
            @Override
            public void run() {
                String outPath = MainActivity.this.getCacheDir().getAbsolutePath() + "/beijing.mp4";
                // due to AssetFileDescriptor.getStartOffset!=0 when use assets or raw, decode fail
                try (InputStream is = getResources().getAssets().open("beijing.mp4")) {
                    IOUtils.copy(is, new FileOutputStream(outPath));
                } catch (IOException e) {
                    e.printStackTrace();
                }

                try (ParcelFileDescriptor pfd = ParcelFileDescriptor.open(
                        new File(outPath), ParcelFileDescriptor.parseMode("r"));) {
                    int fd = pfd.getFd();
                    bitmap = Bitmap.createBitmap(200, 200, Bitmap.Config.ARGB_8888);
                    //you can use it at Glide to replace VideoDecoder
                    VideoSnap snap = new VideoSnap();
                    boolean result = snap.getScaledSnap(fd, bitmap, 200, 200);
                    handler.sendEmptyMessage(message);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }.start();
    }
}