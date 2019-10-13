using System;
using Gtk;
using System.Linq;
using System.Collections.Generic;
using OpenQA.Selenium.Chrome;
using OpenQA.Selenium;
using System.IO;
using System.Text;

public partial class MainWindow : Gtk.Window
{
    public MainWindow() : base(Gtk.WindowType.Toplevel) => Build();

    protected void OnDeleteEvent(object sender, DeleteEventArgs a)
    {
        Application.Quit();
        a.RetVal = true;
    }
    protected void ClearEntrys()
    {
        entry1.Text = string.Empty;
        entry2.Text = string.Empty;
    }

    protected ChromeOptions InitOptions()
    {
        ChromeOptions options = new ChromeOptions();
        options.AddArgument("--incognito");
        options.AddArgument("--start-maximized");
        return options;
    }
    protected void Login(ChromeDriver driver, string login, string password)
    {
        driver.FindElementById("index_email").SendKeys(login);
        driver.FindElementById("index_pass").SendKeys(password);
        driver.FindElementById("index_login_button").SendKeys(Keys.Enter);
        //driver.Manage().Timeouts().ImplicitWait = TimeSpan.FromSeconds(10);
    }

    protected void FindText(List<IWebElement> Elements, List<string> TextElements, int item)
    {
        if (Elements[item].FindElements(By.ClassName("wall_post_text")).Count > 0)
        {
            TextElements.Add(Elements[item].FindElements(By.ClassName("wall_post_text"))[0].Text);
        }
        else
        {
            TextElements.Add("NO TEXT");
        }
    }

    protected void FindImagesURL(List<IWebElement> Thumbs, List<string> ImagesLinks, string imagesWrap, string videoWrap)
    {
        string TempListImages = "";
        foreach (var images in Thumbs)
        {
            foreach (var imageBI in images.FindElements(By.ClassName(imagesWrap)))
            {
                if (!imageBI.GetAttribute("class").Contains(videoWrap))
                {
                    TempListImages += imageBI.GetCssValue("background-image").Substring(5, imageBI.GetCssValue("background-image").Length-7) + "\n";
                }
            }
        }
        if (TempListImages.Length != 0)
        {
            ImagesLinks.Add(TempListImages);
        }
        else
            ImagesLinks.Add("NO IMAGES");
    }

    protected void FindVideosURL(List<IWebElement> Thumbs, List<string> VideosLinks, string imagesWrap, string videoWrap)
    {
        string TempListVideos = "";
        foreach (var videos in Thumbs)
        {
            if (videos.FindElements(By.ClassName(videoWrap)).Count > 0)
            {
                foreach (var dataVideo in videos.FindElements(By.ClassName(videoWrap)))
                {
                    TempListVideos += "https://vk.com/video" + dataVideo.GetAttribute("data-video") + "\n";
                }
            }
        }
        if (TempListVideos.Length != 0)
        {
            VideosLinks.Add(TempListVideos);
        }
        else
            VideosLinks.Add("NO VIDEOS");
    }

    protected void FindAudiosURL(List<IWebElement> Audios, List<string> AudiosLinks, string audioRow)
    {
        string TempListAudios = "";
        foreach (var dataAudio in Audios[0].FindElements(By.ClassName(audioRow)))
        {
            TempListAudios += "https://vk.com/audio" + dataAudio.GetAttribute("data-full-id") + "\n";
        }
        AudiosLinks.Add(TempListAudios);
    }

    protected void FindThumbs(List<IWebElement> Elements, List<string> ImagesLinks, List<string> VideosLinks, List<string> AudiosLinks, int item)
    {
        string imagesWrap = "page_post_thumb_wrap";
        string videoWrap = "page_post_thumb_video";
        string audioRow = "audio_row";
        List<IWebElement> Thumbs = new List<IWebElement>();
        List<IWebElement> Audios = new List<IWebElement>();
        Thumbs = Elements[item].FindElements(By.ClassName("page_post_sized_thumbs")).Distinct().ToList();
        Audios = Elements[item].FindElements(By.ClassName("wall_audio_rows")).Distinct().ToList();
        if (Thumbs.Count > 0)
        {
            FindImagesURL(Thumbs, ImagesLinks, imagesWrap, videoWrap);
            FindVideosURL(Thumbs, VideosLinks, imagesWrap, videoWrap);
        }
        else
        {
            ImagesLinks.Add("NO IMAGES");
            VideosLinks.Add("NO VIDEOS");
        }
        if (Audios.Count > 0)
        {
            FindAudiosURL(Audios, AudiosLinks, audioRow);
        }
        else
        {
            AudiosLinks.Add("NO AUDIOS");
        }
    }

    protected void OnButton2Clicked(object sender, EventArgs e)
    {
        string login = entry1.Text;
        string password = entry2.Text;
        string writePath = @"/home/r3pl1c4nt/Docs/a.txt";

        //FileStream aFile = new FileStream(writePath, FileMode.OpenOrCreate);
        StreamWriter sw = new StreamWriter(writePath, false);
        List<IWebElement> Elements = new List<IWebElement>();
        List<string> PostID = new List<string>();
        List<string> TextElements = new List<string>();
        List<string> ImagesLinks = new List<string>();
        List<string> VideosLinks = new List<string>();
        List<string> AudiosLinks = new List<string>();
        ChromeDriver driver = new ChromeDriver(InitOptions());
        IJavaScriptExecutor jsExecutor = driver;

        ClearEntrys();
        driver.Navigate().GoToUrl("https://vk.com");
        Login(driver, login, password);
        //driver.Navigate().GoToUrl("https://vk.com/feed");
        while(true)
        {
            int oldCount = Elements.Count;
            jsExecutor.ExecuteScript("window.scrollTo(0, document.body.scrollHeight);");
            Elements = driver.FindElementsByCssSelector(".wall_post_cont").Distinct().ToList();
            for(var item = oldCount; item != Elements.Count; ++item)
            {
                PostID.Add("https://vk.com/wall" + Elements[item].GetAttribute("id").Substring(3, Elements[item].GetAttribute("id").Length - 3));
                FindText(Elements, TextElements, item);
                FindThumbs(Elements, ImagesLinks, VideosLinks, AudiosLinks, item);
            }

            for (int i = oldCount; i < Elements.Count; ++i)
            {
                //sw.WriteLine(Elements[i].Text);
                sw.WriteLine("-----id-----");
                sw.WriteLine(PostID[i]);
                sw.WriteLine("-----text-----");
                sw.WriteLine(TextElements[i]);
                sw.WriteLine("-----images-----");
                sw.WriteLine(ImagesLinks[i]);
                sw.WriteLine("-----videos-----");
                sw.WriteLine(VideosLinks[i]);
                sw.WriteLine("-----audios-----");
                sw.WriteLine(AudiosLinks[i]);
                sw.WriteLine("+++++end+++++");
            }
        }

        
        //

    }
}
